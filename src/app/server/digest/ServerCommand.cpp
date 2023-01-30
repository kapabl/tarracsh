#include "ServerCommand.h"
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>


using namespace kapa::tarracsh::app::server::digest;

using kapa::tarracsh::server::digest::ServiceImpl;

ServerCommand::ServerCommand(Context &appConfig)
    : _appConfig(appConfig) {

    const auto channel = grpc::CreateChannel(_appConfig.getOptions().digest.client.getServerAddress(),
                                             grpc::InsecureChannelCredentials());

    _stub = PublicDigest::NewStub(channel);
}

bool ServerCommand::run(Context &appConfig) {
    const ServerCommand serverCommand(appConfig);
    const auto options = appConfig.getOptions();
    auto result = false;
    if (options.digest.server.stopServer) {
        result = serverCommand.stop();
    } else if (options.digest.client.isClientMode) {
        result = serverCommand.diff();
    } else {
        result = serverCommand.start();
    }

    return result;
}


bool ServerCommand::stop() const {
    grpc::ClientContext context;
    const Empty request;
    Empty response;
    const auto status = _stub->Quit(&context, request, &response);
    const auto result = status.error_code() == grpc::StatusCode::OK;
    if (!result) {
        std::cout << format("Error: {} - {} ", static_cast<int>(status.error_code()), status.error_message()) <<
            std::endl;
    }
    return result;

}

bool ServerCommand::start() const {
    const auto result = ServiceImpl::start(_appConfig);
    return result;
}

void ServerCommand::optionsToRequest(DiffRequest &request) const {
    const auto options = _appConfig.getOptions();
    request.set_input(options.digest.input);
    request.set_dryrun(options.digest.dryRun);
}

void ServerCommand::responseToReport(const DiffResponse &response) const {

    const auto &report = _appConfig.getResults().report;

    for (auto &diff : response.files()) {
        domain::stats::report::FileResult fileResult;
        fileResult.failed = diff.failed();
        fileResult.filename = diff.filename();
        fileResult.isSamePublicDigest = diff.issamedigest();
        fileResult.isModified = diff.ismodified();
        fileResult.isNew = diff.isnew();
        report->addFile(fileResult);
    }

    for (auto &diff : response.classfiles()) {
        domain::stats::report::ClassResult classResult;
        classResult.failed = diff.failed();
        classResult.strongClassname = diff.strongclassname();
        classResult.isSamePublicDigest = diff.issamedigest();
        classResult.isModified = diff.ismodified();
        classResult.isNew = diff.isnew();
        report->addClass(classResult);
        
    }
}



bool ServerCommand::diff() const {
    grpc::ClientContext context;
    DiffRequest request;
    optionsToRequest(request);
    DiffResponse response;
    const auto status = _stub->Diff(&context, request, &response);

    const auto result = status.ok();
    if (result) {
        responseToReport(response);
        // _appConfig.getResults().report->print();
    } else {
        _appConfig.getLog().writeln(std::format("Server Request Error: {}", status.error_message()), true);
    }

    return result;
}
