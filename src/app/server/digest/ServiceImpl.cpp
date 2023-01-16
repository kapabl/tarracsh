#include "ServiceImpl.h"

#include <iostream>
#include <ostream>
#include <chrono>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <grpcpp/server_builder.h>

#include "ServerCommand.h"
#include "../RequestConfig.h"
#include "../app/Analyzer.h"
#include "../domain/stats/ScopedTimer.h"


using kapa::tarracsh::server::digest::ServiceImpl;
using kapa::tarracsh::domain::stats::profiler::MillisecondDuration;
using kapa::tarracsh::domain::stats::profiler::ScopedTimer;
using kapa::tarracsh::domain::stats::report::DigestReport;
using kapa::tarracsh::app::server::digest::DigestRequest;
using kapa::tarracsh::app::server::digest::DigestResponse;
using kapa::tarracsh::app::server::digest::Empty;



using namespace grpc;
using namespace std;
using namespace chrono;
using namespace boost::interprocess;


using grpc::ServerBuilder;

#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

const char *PUBLIC_DIGEST_SERVER_MUTEX_NAME = "public-digest-grpc-server";

void ServiceImpl::start(app::Config& config) {
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

    auto result = true;
    domain::stats::profiler::MillisecondDuration duration{0};
    {
        ScopedTimer timer(&duration);
        _db.init();

        if (_config.getOptions().rebuild) {
            _db.clean();
        } else {
            result = _db.read();
        }
    }

    cout << std::format("Db init duration:{:10L}", duration) << endl;
    _db.outputStats();

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
    _cv.wait(lock, [this] {
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
    if ( !requestOptions.processInput()) {
        // TODO _results.log
        cout << format("Invalid Input: {}", requestOptions.input) << endl;
    }

}

void ServiceImpl::reportToResponse(const std::unique_ptr<DigestReport> &report,DigestResponse &response) {
    //TODO
}

ServiceImpl::ServiceImpl(app::Config& config)
    : _db(config.getOptions().outputDir, config.getLog()),
      _config(config) {
}

Status ServiceImpl::Quit(ServerContext *context, const Empty *request, Empty *response) {
    _quickReceived = true;
    _cv.notify_one();
    return Status::OK;
}

Status ServiceImpl::Check(ServerContext *context, const DigestRequest *request, DigestResponse *response) {
    Status result(Status::OK);
    RequestConfig requestConfig(app::App::getApp().getLog());
    requestToOptions(*request, requestConfig.getOptions());
    app::Analyzer analyzer(requestConfig);
    analyzer.run();
    reportToResponse(requestConfig.getResults().report, *response);
    return result;
}

