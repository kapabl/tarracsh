#ifndef TARRACSH_SERVER_H
#define TARRACSH_SERVER_H
#include "../app/Options.h"
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include "proto/Server.grpc.pb.h"
#include "../tables/DigestDb.h"

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;

namespace org::kapa::tarracsh::server::digest {


class ServiceImpl: public PublicDigest::Service {
public:
    static void start(app::Config& config);

    explicit ServiceImpl(app::Config& config);

    Status Quit(ServerContext* context, const Empty* request, Empty* response) override;
    Status Check(ServerContext* context, const DigestRequest* request, DigestResponse* response) override;

    Status GetFeature(grpc::ServerContext* context, const Point* point,
                      Feature* feature) override;


    Status ListFeatures(grpc::ServerContext* context,
                        const Rectangle* rectangle,
                        grpc::ServerWriter<Feature>* writer) override;

    Status RecordRoute(ServerContext* context, ServerReader<Point>* reader,
                       RouteSummary* summary) override;

    Status RouteChat(ServerContext* context,
                     ServerReaderWriter<RouteNote, RouteNote>* stream) override;



private:
    db::DigestDb _db;

    std::vector<Feature> feature_list_;
    std::mutex mu_;
    std::vector<RouteNote> received_notes_;

    // const Options& _options;
    app::Config& _config;
    std::unique_ptr<grpc::Server> _server;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _quickReceived{ false };

    bool initDb();
    void startServer();
    void waitForShutDown();
    void init();
    void requestToOptions(const DigestRequest& request, Options& requestOptions) const;
    void reportToResponse(const std::unique_ptr<stats::report::Report>& report, DigestResponse& response);

    std::string GetFeatureName(const Point& point,
                               const std::vector<Feature>& feature_list) {
        for (const Feature& f : feature_list) {
            if (f.location().latitude() == point.latitude() &&
                f.location().longitude() == point.longitude()) {
                return f.name();
            }
        }
        return "";
    }


    float ConvertToRadians(float num) { return num * 3.1415926 / 180; }

    // The formula is based on http://mathforum.org/library/drmath/view/51879.html
    float GetDistance(const Point& start, const Point& end) {
        const float kCoordFactor = 10000000.0;
        float lat_1 = start.latitude() / kCoordFactor;
        float lat_2 = end.latitude() / kCoordFactor;
        float lon_1 = start.longitude() / kCoordFactor;
        float lon_2 = end.longitude() / kCoordFactor;
        float lat_rad_1 = ConvertToRadians(lat_1);
        float lat_rad_2 = ConvertToRadians(lat_2);
        float delta_lat_rad = ConvertToRadians(lat_2 - lat_1);
        float delta_lon_rad = ConvertToRadians(lon_2 - lon_1);

        float a = pow(sin(delta_lat_rad / 2), 2) +
            cos(lat_rad_1) * cos(lat_rad_2) * pow(sin(delta_lon_rad / 2), 2);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        int R = 6371000;  // metres

        return R * c;
    }
};
}


#endif
