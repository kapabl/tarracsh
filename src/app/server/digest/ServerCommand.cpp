#include "ServerCommand.h"
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

using namespace kapa::tarracsh;
using namespace app;
using namespace server::digest;
using namespace std;




ServerCommand::ServerCommand(Config& config):_config(config) {

    const auto channel = grpc::CreateChannel(_config.getOptions().digestServer.getServerAddress(), grpc::InsecureChannelCredentials());

    _stub = PublicDigest::NewStub(channel);
}

void ServerCommand::run(Config& config) {
    const ServerCommand serverCommand(config);
    if ( config.getOptions().digestServer.stopServer ) {
        serverCommand.stop();
    } else {
        serverCommand.start();
    }
}


void ServerCommand::stop() const {
    grpc::ClientContext context;
    const Empty request;
    Empty response;
    const auto status = _stub->Quit( &context, request, &response );
    if (status.error_code() != grpc::StatusCode::OK) {
        cout << format("Error: {} - {} ", static_cast<int>(status.error_code()), status.error_message()) << endl;
    }
   

}

void ServerCommand::start() const {
    ServiceImpl::start(_config);
}
