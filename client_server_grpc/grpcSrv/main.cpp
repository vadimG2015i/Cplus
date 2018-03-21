#include <QCoreApplication>


#define _WIN32_WINNT 0x0600

#include <grpc/grpc.h>
#include <grpc++/grpc++.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/client_context.h>
#include <grpc++/channel.h>
#include <grpc++/security/server_credentials.h>
#include <grpctest.grpc.pb.h>
#include "grpctest.pb.h"
#include <QTime>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;


using GRPCTestApi::ConvertRequest;
using GRPCTestApi::ConvertResponse;

using GRPCTestApi::GetTimeRequest;
using GRPCTestApi::GetTimeResponse;

using GRPCTestApi::GetServerNameRequest;
using GRPCTestApi::GetServerNameResponse;
using GRPCTestApi::GRPCTestApiSrv;

/*
 * rpc GetTime(GetTimeRequest) returns (GetTimeResponse) {}
  rpc GetServerName(GetServerNameRequest ) returns (GetServerNameResponse) {}
  rpc Convert(ConvertRequest) returns (ConvertResponse) {}

 */

class GRPCTestApiImpl final : public GRPCTestApiSrv::Service {

  Status GetServerName(ServerContext* context, const GetServerNameRequest* request,
                  GetServerNameResponse* reply) override {
    reply->set_name("I am server ");
    return Status::OK;
  }

  Status GetTime(ServerContext* context, const GetTimeRequest* request,
                  GetTimeResponse* reply) override {

    reply->set_currenttime(QTime::currentTime().toString().toStdString());
   // reply->set_url("some_url");

    return Status::OK;
  }

  Status Convert(ServerContext* context, const ConvertRequest* request,
                  ConvertResponse* reply) override {

    QString string = request->conv().c_str();
    std::string res = string.toLatin1().toHex().toStdString();

    reply->set_conv(res);
    return Status::OK;
  }


};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GRPCTestApiImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    RunServer();
    return a.exec();
}
