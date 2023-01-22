#ifndef TARRACSH_SERVER_COMMAND_H
#define TARRACSH_SERVER_COMMAND_H
#include "../app/Context.h"

#include "ServiceImpl.h"
#include "proto/Server.grpc.pb.h"


namespace kapa::tarracsh::app::server::digest {


class ServerCommand {
public:

    [[nodiscard]] static bool run(Context& appConfig);

    [[nodiscard]] bool stop() const;
    [[nodiscard]] bool start() const;
    [[nodiscard]] bool diff() const;


private:

    explicit ServerCommand(Context& appConfig);
    
    Context & _appConfig;
    std::unique_ptr<PublicDigest::Stub> _stub;

    void optionsToRequest(DiffRequest& request) const;
    void responseToReport(const DiffResponse& response) const;

};
}


#endif
