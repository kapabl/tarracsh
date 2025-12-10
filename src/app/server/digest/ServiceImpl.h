#ifndef TARRACSH_SERVER_H
#define TARRACSH_SERVER_H

#include <condition_variable>
#include <functional>
#include <memory>
#include "../../Context.h"
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include "proto/Server.grpc.pb.h"
#include "domain/db/DigestDb.h"

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;

namespace kapa::tarracsh::server::digest {

namespace testhooks {
struct ServiceImplAccessor;
}



class ServiceImpl: public app::server::digest::PublicDigest::Service {
public:
    class ServerHandle {
    public:
        virtual ~ServerHandle() = default;
        virtual void Shutdown() = 0;
    };

    [[nodiscard]] static bool start(app::Context& config);
    static void resetQuickSignalForTests();
    using ServerHandleFactory = std::function<std::unique_ptr<ServerHandle>(ServiceImpl &, const std::string &)>;
    static ServerHandleFactory setServerHandleFactoryForTests(ServerHandleFactory replacement);
    using DigestDbFactory = std::function<std::shared_ptr<domain::db::digest::DigestDb>(
        const domain::db::digest::DigestDb::Config &, bool, bool)>;
    static DigestDbFactory setDigestDbFactoryForTests(DigestDbFactory replacement);

    explicit ServiceImpl(app::Context& context);
    static void signalQuick();

    [[nodiscard]] Status Quit(ServerContext* context,
        const app::server::digest::Empty* request, 
        app::server::digest::Empty* response) override;

    [[nodiscard]] Status Diff(ServerContext* context,
        const app::server::digest::DiffRequest* request, 
        app::server::digest::DiffResponse* response) override;

protected:
    static void reportToResponse(const std::unique_ptr<domain::stats::report::Report>& report,
                                 app::server::digest::DiffResponse& response);

private:
    friend struct testhooks::ServiceImplAccessor;
    std::shared_ptr<domain::db::digest::DigestDb> _db;
    app::Context& _context;
    std::unique_ptr<ServerHandle> _server;
    std::mutex _mutex;
    static std::condition_variable _quickSignalCV;
    static bool _quickReceived;

    static ServerHandleFactory &serverHandleFactory();
    static DigestDbFactory &digestDbFactory();

    bool initDb();
    void startServer();
    void waitForShutDown();
    void init();

};
}

namespace kapa::tarracsh::server::digest::testhooks {
struct ServiceImplAccessor {
    static void startServer(ServiceImpl &impl) { impl.startServer(); }
    static void waitForShutdown(ServiceImpl &impl) { impl.waitForShutDown(); }
    static bool initDb(ServiceImpl &impl) { return impl.initDb(); }
};
}


#endif
