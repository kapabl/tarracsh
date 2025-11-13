#ifndef TARRACSH_SERVER_COMMAND_H
#define TARRACSH_SERVER_COMMAND_H
#include "../../Context.h"

#include "ServiceImpl.h"
#include "proto/Server.grpc.pb.h"
#include <functional>


namespace kapa::tarracsh::app::server::digest {


class ServerCommand {
public:

    [[nodiscard]] static bool run(Context& appConfig);

    [[nodiscard]] bool stop() const;
    [[nodiscard]] bool start() const;
    [[nodiscard]] bool diff() const;
    using StubFactory = std::function<std::unique_ptr<PublicDigest::StubInterface>(Context&)>;
    using ServerStarter = std::function<bool(Context&)>;
    static StubFactory setStubFactoryForTests(StubFactory replacement);
    static ServerStarter setServerStarterForTests(ServerStarter replacement);


private:

    explicit ServerCommand(Context& appConfig);
    
    Context & _appConfig;
    std::unique_ptr<PublicDigest::StubInterface> _stub;

    void optionsToRequest(DiffRequest& request) const;
    void responseToReport(const DiffResponse& response) const;
    static StubFactory &stubFactory();
    static ServerStarter &serverStarter();

};
}


#endif
