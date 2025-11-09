#include "ServiceImpl.h"

#include <iostream>
#include <ostream>
#include <chrono>
#include <fmt/chrono.h>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <grpcpp/server_builder.h>

#include "app/AppRuntime.h"
#include "ServerCommand.h"
#include "app/server/RequestContext.h"
#include "../../Analyzer.h"
#include "infrastructure/profiling/ScopedTimer.h"


using kapa::tarracsh::server::digest::ServiceImpl;
using kapa::tarracsh::domain::db::digest::DigestDb;
using kapa::tarracsh::domain::stats::profiler::MillisecondDuration;
using kapa::infrastructure::profiler::ScopedTimer;
using kapa::tarracsh::domain::stats::report::Report;
using kapa::tarracsh::app::server::digest::DiffRequest;
using kapa::tarracsh::app::server::digest::DiffResponse;
using kapa::tarracsh::app::server::digest::FileDigestResult;
using kapa::tarracsh::app::server::digest::Empty;


using namespace grpc;
using namespace std;
using namespace chrono;
using namespace boost::interprocess;


using grpc::ServerBuilder;


bool ServiceImpl::_quickReceived = false;
std::condition_variable ServiceImpl::_quickSignalCV;

const char *PUBLIC_DIGEST_SERVER_MUTEX_NAME = "public-digest-grpc-server";

bool ServiceImpl::start(app::Context &config) {
    named_mutex::remove(PUBLIC_DIGEST_SERVER_MUTEX_NAME);
    named_mutex mutex(open_or_create, PUBLIC_DIGEST_SERVER_MUTEX_NAME);

    const auto result = mutex.try_lock();
    if (result) {
        ServiceImpl service(config);
        service.init();
        mutex.unlock();
    } else {
        cout << "Public Digest Server is already running";
    }

    return result;
}

bool ServiceImpl::initDb() {

    //TODO start saving db thread

    auto result = false;
    MillisecondDuration duration{0};
    {
        ScopedTimer timer(&duration);
        _db = DigestDb::create(
            {_context.getOptions().outputDir,
             &_context.getLog()},
            false,
            true);
    }
    cout << fmt::format("Db init duration:{}", duration) << endl;
    if (_db) {
        _db->outputStats();
        result = true;
    } else {
        cout << "Error initializing Digest db" << endl;
    }

    return result;

}

void ServiceImpl::startServer() {
    const auto serverAddress = _context.getOptions().digest.server.getListenServerAddress();

    ServerBuilder builder;
    builder.AddListeningPort(serverAddress, InsecureServerCredentials())
           .RegisterService(this);

    _server = builder.BuildAndStart();
    cout << "Server listening on " << serverAddress << endl << endl;
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
        _db->stop();

    }

}

void ServiceImpl::reportToResponse(const std::unique_ptr<Report> &report, DiffResponse &response) {

    for (const auto &fileResult : report->getFileResults()) {
        const auto fileDigestResult = response.add_files();
        fileDigestResult->set_filename(fileResult.filename);
        fileDigestResult->set_ismodified(fileResult.isModified);
        fileDigestResult->set_isnew(fileResult.isNew);
        fileDigestResult->set_issamedigest(fileResult.isSamePublicDigest);
        fileDigestResult->set_failed(fileResult.failed);
    }

    for (const auto &classResult : report->getClassResults()) {
        const auto classDigestResult = response.add_classfiles();
        classDigestResult->set_strongclassname(classResult.strongClassname);
        classDigestResult->set_ismodified(classResult.isModified);
        classDigestResult->set_isnew(classResult.isNew);
        classDigestResult->set_issamedigest(classResult.isSamePublicDigest);
        classDigestResult->set_failed(classResult.failed);
    }

}

ServiceImpl::ServiceImpl(app::Context &context)
    : _context(context) {
}

void ServiceImpl::signalQuick() {
    _quickReceived = true;
    _quickSignalCV.notify_one();
}

Status ServiceImpl::Quit(ServerContext *context, const Empty *request, Empty *response) {
    signalQuick();
    return Status::OK;
}

Status ServiceImpl::Diff(ServerContext *context, const DiffRequest *request, DiffResponse *response) {
    Status result(Status::OK);
    ScopedTimer::timeWithPrint(
        "server-diff",
        [this, request, response, &result]() -> void {
            RequestContext requestContext;
            if (requestContext.update(*request)) {
                //TODO check what analyzer to use?
                app::Analyzer analyzer(requestContext, _db);
                analyzer.run();
                requestContext.getResults().report->print();
                reportToResponse(
                    requestContext.getResults().report, *response);
            } else {
                result = Status(
                    StatusCode::INVALID_ARGUMENT,
                    requestContext.getErrorMessage());
            }
        });

    return result;
}
