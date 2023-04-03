#ifndef TARRACSH_SERVER_H
#define TARRACSH_SERVER_H
#include "../../Context.h"
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include "proto/Server.grpc.pb.h"
#include "../../../domain/db/DigestDb.h"

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;

namespace kapa::tarracsh::server::digest {



class ServiceImpl: public app::server::digest::PublicDigest::Service {
public:
    [[nodiscard]] static bool start(app::Context& config);

    explicit ServiceImpl(app::Context& context);
    static void signalQuick();

    [[nodiscard]] Status Quit(ServerContext* context,
        const app::server::digest::Empty* request, 
        app::server::digest::Empty* response) override;

    [[nodiscard]] Status Diff(ServerContext* context,
        const app::server::digest::DiffRequest* request, 
        app::server::digest::DiffResponse* response) override;


private:
    std::shared_ptr<domain::db::digest::DigestDb> _db;
    app::Context& _context;
    std::unique_ptr<grpc::Server> _server;
    std::mutex _mutex;
    static std::condition_variable _quickSignalCV;
    static bool _quickReceived;

    bool initDb();
    void startServer();
    void waitForShutDown();
    void init();
    static void reportToResponse(const std::unique_ptr<domain::stats::report::DigestReport>& report, app::server::digest::DiffResponse& response);

};
}


#endif
