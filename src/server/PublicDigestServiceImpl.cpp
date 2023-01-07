#include "PublicDigestServiceImpl.h"

#include <iostream>
#include <ostream>
#include <chrono>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>


using namespace grpc;
using namespace org::kapa::tarracsh::server;
using namespace std;
using namespace chrono;
using namespace boost::interprocess;



#ifdef _WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

const char* PUBLIC_DIGEST_SERVER_MUTEX_NAME = "public-digest-grpc-server";

void PublicDigestServiceImpl::start(const Options& options) {
    named_mutex mutex(open_or_create, PUBLIC_DIGEST_SERVER_MUTEX_NAME);
    if (mutex.try_lock()) {
        PublicDigestServiceImpl service(options);
        mutex.unlock();
    } else {
        cout << "Public Digest Server is already running";
    }
}

PublicDigestServiceImpl::PublicDigestServiceImpl(const Options &options): _options(options) {
    const auto serverAddress(format("0.0.0.0:{}", options.port));

    ServerBuilder builder;
    builder.AddListeningPort(serverAddress, InsecureServerCredentials())
        .RegisterService(this);

    const unique_ptr server(builder.BuildAndStart());
    cout << "Server listening on " << serverAddress << endl;
    server->Wait();
}

Status PublicDigestServiceImpl::GetFeature(grpc::ServerContext *context, const Point *point, Feature *feature) {
    feature->set_name(GetFeatureName(*point, feature_list_));
    feature->mutable_location()->CopyFrom(*point);
    return Status::OK;
}

Status PublicDigestServiceImpl::ListFeatures(grpc::ServerContext *context, const Rectangle *rectangle,
    grpc::ServerWriter<Feature> *writer) {
    auto lo = rectangle->lo();
    auto hi = rectangle->hi();
    long left = (min)(lo.longitude(), hi.longitude());
    long right = (max)(lo.longitude(), hi.longitude());
    long top = (max)(lo.latitude(), hi.latitude());
    long bottom = (min)(lo.latitude(), hi.latitude());
    for (const Feature& f : feature_list_) {
        if (f.location().longitude() >= left &&
            f.location().longitude() <= right &&
            f.location().latitude() >= bottom && f.location().latitude() <= top) {
            writer->Write(f);
        }
    }
    return Status::OK;
}

Status PublicDigestServiceImpl::
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

Status PublicDigestServiceImpl::RouteChat(ServerContext *context, ServerReaderWriter<RouteNote, RouteNote> *stream) {
    RouteNote note;
    while (stream->Read(&note)) {
        unique_lock<mutex> lock(mu_);
        for (const RouteNote& n : received_notes_) {
            if (n.location().latitude() == note.location().latitude() &&
                n.location().longitude() == note.location().longitude()) {
                stream->Write(n);
            }
        }
        received_notes_.push_back(note);
    }

    return Status::OK;
}
