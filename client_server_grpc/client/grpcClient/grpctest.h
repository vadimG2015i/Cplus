#ifndef GRPCTEST_H
#define GRPCTEST_H
#include "grpctest.pb.h"
#include <QMainWindow>

#include <grpc/grpc.h>
#include <grpc++/grpc++.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/client_context.h>
#include <grpc++/channel.h>
#include <grpc++/security/server_credentials.h>
#include <grpctest.grpc.pb.h>

#include <iostream>
#include <memory>
#include <string>

using grpc::Channel;
using grpc::ChannelArguments;
using grpc::ClientContext;
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

class GRPCTestClient {
 public:
  GRPCTestClient(std::shared_ptr<Channel> channel)
      : stub_(GRPCTestApiSrv::NewStub(channel)) {}

  std::string GetTime(const std::string& name)
  {
    GetTimeRequest request;
    request.set_currenttime(name);
    GetTimeResponse reply;
    ClientContext context;

    Status status = stub_->GetTime(&context, request, &reply);
    if (status.ok())
    {
      return reply.currenttime();
    }
    else
    {
      return std::string("not respond");
    }
  }

  std::string GetServerName(const std::string& name)
  {
    GetServerNameRequest request;
    request.set_name(name);
    GetServerNameResponse reply;
    ClientContext context;

    Status status = stub_->GetServerName(&context, request, &reply);
    if (status.ok())
    {
      return reply.name();
    }
    else
    {
      return std::string("not respond");
    }
  }


  std::string Convert(const std::string& name)
  {
    ConvertRequest request;
    request.set_conv(name);
    ConvertResponse reply;
    ClientContext context;

    Status status = stub_->Convert(&context, request, &reply);
    if (status.ok())
    {
      return reply.conv();
    }
    else
    {
       return std::string("not respond");
    }
  }




 private:
  std::unique_ptr<GRPCTestApiSrv::Stub> stub_;
};




namespace Ui {
class grpcTest;
}


class grpcTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit grpcTest(QWidget *parent = 0);
    GRPCTestClient *client;
    ~grpcTest();

private:
    Ui::grpcTest *ui;

private slots:

 void GetTimeButton_cliked();
 void GetName_cliked();
 void ConvertString_cliked();
};

#endif // GRPCTEST_H
