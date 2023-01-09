#ifndef TARRACSH_SERVER_COMMAND_H
#define TARRACSH_SERVER_COMMAND_H
#include "../app/Config.h"

#include "ServiceImpl.h"
#include "proto/Server.grpc.pb.h"


namespace org::kapa::tarracsh::server::digest {


class ServerCommand {
public:

    static void run(app::Config& config);

    void stop() const;
    void start() const;


private:

    explicit ServerCommand(app::Config& config);

    app::Config & _config;
    std::unique_ptr<PublicDigest::Stub> _stub;

};
}


#endif
