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
#include "../app/stats/ScopedTimer.h"


using namespace grpc;
using namespace org::kapa::tarracsh::server::digest;
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
    stats::profiler::MillisecondDuration duration{0};
    {
        stats::profiler::ScopedTimer timer(&duration);
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

void ServiceImpl::requestToOptions(const DigestRequest &request, Options &requestOptions) const {
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

void ServiceImpl::reportToResponse(const std::unique_ptr<stats::report::Report> &report, DigestResponse &response) {
    //TODO
}

ServiceImpl::ServiceImpl(app::Config& config)
    : _db(config.getOptions().outputDir),
      _config(config) {
}

Status ServiceImpl::Quit(ServerContext *context, const Empty *request, Empty *response) {
    _quickReceived = true;
    _cv.notify_one();
    return Status::OK;
}

Status ServiceImpl::Check(ServerContext *context, const DigestRequest *request, DigestResponse *response) {
    Status result(Status::OK);
    RequestConfig requestConfig;
    requestToOptions(*request, requestConfig.getOptions());
    app::Analyzer analyzer(requestConfig);
    analyzer.run();
    reportToResponse(requestConfig.getResults().report, *response);
    return result;
}

Status ServiceImpl::GetFeature(grpc::ServerContext *context, const Point *point, Feature *feature) {
    feature->set_name(GetFeatureName(*point, feature_list_));
    feature->mutable_location()->CopyFrom(*point);
    return Status::OK;
}

Status ServiceImpl::ListFeatures(grpc::ServerContext *context, const Rectangle *rectangle,
                                 grpc::ServerWriter<Feature> *writer) {
    auto lo = rectangle->lo();
    auto hi = rectangle->hi();
    long left = (min)(lo.longitude(), hi.longitude());
    long right = (max)(lo.longitude(), hi.longitude());
    long top = (max)(lo.latitude(), hi.latitude());
    long bottom = (min)(lo.latitude(), hi.latitude());
    for (const Feature &f : feature_list_) {
        if (f.location().longitude() >= left &&
            f.location().longitude() <= right &&
            f.location().latitude() >= bottom && f.location().latitude() <= top) {
            writer->Write(f);
        }
    }
    return Status::OK;
}

Status ServiceImpl::
RecordRoute(ServerContext *context, ServerReader<Point> *reader, RouteSummary *summary) {
    Point point;
    int point_count = 0;
    int feature_count = 0;
    float distance = 0.0;
    Point previous;

    system_clock::time_point start_time = system_clock::now();
    while (reader->Read(&point)) {
        point_count++;
        if (!GetFeatureName(point, feature_list_).empty()) {
            feature_count++;
        }
        if (point_count != 1) {
            distance += GetDistance(previous, point);
        }
        previous = point;
    }
    system_clock::time_point end_time = system_clock::now();
    summary->set_point_count(point_count);
    summary->set_feature_count(feature_count);
    summary->set_distance(static_cast<long>(distance));
    auto secs = duration_cast<seconds>(end_time - start_time);
    summary->set_elapsed_time(secs.count());

    return Status::OK;
}

Status ServiceImpl::RouteChat(ServerContext *context, ServerReaderWriter<RouteNote, RouteNote> *stream) {
    RouteNote note;
    while (stream->Read(&note)) {
        unique_lock<mutex> lock(mu_);
        for (const RouteNote &n : received_notes_) {
            if (n.location().latitude() == note.location().latitude() &&
                n.location().longitude() == note.location().longitude()) {
                stream->Write(n);
            }
        }
        received_notes_.push_back(note);
    }

    return Status::OK;
}
