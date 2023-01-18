#include "ServiceImpl.h"

#include <iostream>
#include <ostream>
#include <chrono>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <grpcpp/server_builder.h>

#include "../../App.h"
#include "ServerCommand.h"
#include "../RequestConfig.h"
#include "../app/Analyzer.h"
#include "../domain/stats/ScopedTimer.h"


using kapa::tarracsh::server::digest::ServiceImpl;
using kapa::tarracsh::domain::db::digest::DigestDb;
using kapa::tarracsh::domain::stats::profiler::MillisecondDuration;
using kapa::tarracsh::domain::stats::profiler::ScopedTimer;
using kapa::tarracsh::domain::stats::report::DigestReport;
using kapa::tarracsh::app::server::digest::DigestRequest;
using kapa::tarracsh::app::server::digest::DigestResponse;
using kapa::tarracsh::app::server::digest::FileDigestResult;
using kapa::tarracsh::app::server::digest::Empty;


using namespace grpc;
using namespace std;
using namespace chrono;
using namespace boost::interprocess;


using grpc::ServerBuilder;


bool ServiceImpl::_quickReceived = false;
std::condition_variable ServiceImpl::_quickSignalCV;

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

const char *PUBLIC_DIGEST_SERVER_MUTEX_NAME = "public-digest-grpc-server";

void ServiceImpl::start(app::Config &config) {
    named_mutex mutex(open_or_create, PUBLIC_DIGEST_SERVER_MUTEX_NAME);
    if (mutex.try_lock()) {
        ServiceImpl service(config);
        service.init();
        mutex.unlock();
    } else {
        cout << "Public Digest Server is already running";
    }
}

bool ServiceImpl::initDb() {

    auto result = false;
    MillisecondDuration duration{0};
    {
        ScopedTimer timer(&duration);
        _db = DigestDb::create(_config.getOptions().outputDir, _config.getLog(), false);

        cout << std::format("Db init duration:{}", duration) << endl;
        if (_db) {
            _db->outputStats();
            result = true;
        }
        else {
            cout << "Error initializing Digest db" << endl;
        }
    }
    return result;

}

void ServiceImpl::startServer() {
    const auto serverAddress = _config.getOptions().digestServer.getListenServerAddress();

    ServerBuilder builder;
    builder.AddListeningPort(serverAddress, InsecureServerCredentials())
           .RegisterService(this);

    _server = builder.BuildAndStart();
    cout << "Server listening on " << serverAddress << endl;
    // _server->Wait();
    waitForShutDown();
    _server->Shutdown();

}

void ServiceImpl::waitForShutDown() {
    unique_lock lock(_mutex);
    _quickSignalCV.wait(lock, [this] {
        return _quickReceived;
    });

}

void ServiceImpl::init() {
    cout << "Initializing..." << endl;
    if (initDb()) {
        startServer();
    }

}

void ServiceImpl::requestToOptions(const DigestRequest &request, domain::Options &requestOptions) const {
    /*
     * struct Options {
    std::string classFilePath;
    std::string input;
    std::string directory;
    std::string jarFile;
    std::string classPath;
    std::string queryValue;
    std::string outputDir;
    bool isParse{false};
    bool isPublicDigest{false};
    bool isCallGraph{false};
    bool printClassParse{false};
    bool printConstantPool{false};
    bool rebuild{false};
    bool dryRun{false};
    bool doDiffReport{true};
    bool printDiffReport{false};
    std::string logFile;
    int workers{4};
    bool useFileTimestamp{true};
    bool pause{false};
    bool printProfiler{false};
    bool printCPoolHtmlNav{false};
    bool descriptiveCPoolEntries{true};
    bool verbose{false};
    ServerOptions digestServer


}
     */
    requestOptions.isServerMode = true;
    requestOptions.isPublicDigest = true;
    requestOptions.logFile = _config.getOptions().logFile;

    requestOptions.input = request.input();
    if (!requestOptions.processInput()) {
        // TODO _results.log
        cout << format("Invalid Input: {}", requestOptions.input) << endl;
    }

}

//TODO write ResponseToReport
void ServiceImpl::reportToResponse(const std::unique_ptr<DigestReport> &report, DigestResponse &response) {
  

    for( const auto& jarResult: report->getJarResults() ) {
        auto jarDigestResult = *response.add_jars();
        jarDigestResult.set_filename(jarResult.filename);
        jarDigestResult.set_ischanged(jarResult.isModified);
        jarDigestResult.set_isnew(jarResult.isNew);
        jarDigestResult.set_issamedigest(jarResult.isSamePublicDigest);
    }

    for (const auto& classResults : report->getClassResults()) {
        auto classDigestResult = *response.add_classfiles();
        classDigestResult.set_filename(classResults.strongClassname);
        classDigestResult.set_ischanged(classResults.isModified);
        classDigestResult.set_isnew(classResults.isNew);
        classDigestResult.set_issamedigest(classResults.isSamePublicDigest);
    }
 
}

ServiceImpl::ServiceImpl(app::Config &config)
    : _config(config) {
    initDb();
}

void ServiceImpl::signalQuick() {
    _quickReceived = true;
    _quickSignalCV.notify_one();
}

Status ServiceImpl::Quit(ServerContext *context, const Empty *request, Empty *response) {
    signalQuick();
    return Status::OK;
}

Status ServiceImpl::Check(ServerContext *context, const DigestRequest *request, DigestResponse *response) {
    Status result(Status::OK);
    RequestConfig requestConfig(app::App::getApp().getLog());
    requestToOptions(*request, requestConfig.getOptions());

    app::Analyzer analyzer(requestConfig, _db);
    analyzer.run();

    reportToResponse(requestConfig.getResults().report, *response);
    return result;
}
