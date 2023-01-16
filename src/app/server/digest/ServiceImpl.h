#ifndef TARRACSH_SERVER_H
#define TARRACSH_SERVER_H
#include "../app/Config.h"
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include "proto/Server.grpc.pb.h"
#include "../domain/db/DigestDb.h"

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;

namespace kapa::tarracsh::server::digest {


class ServiceImpl: public PublicDigest::Service {
public:
    static void start(app::Config& config);

    explicit ServiceImpl(app::Config& config);

    Status Quit(ServerContext* context, const Empty* request, Empty* response) override;
    Status Check(ServerContext* context, const DigestRequest* request, DigestResponse* response) override;


private:
    db::digest::DigestDb _db;
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
    void reportToResponse(const std::unique_ptr<stats::report::DigestReport>& report, DigestResponse& response);
};
}


#endif
