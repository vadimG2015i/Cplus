// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: grpctest.proto
#ifndef GRPC_grpctest_2eproto__INCLUDED
#define GRPC_grpctest_2eproto__INCLUDED

#include "grpctest.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace GRPCTestApi {

class GRPCTestApiSrv final {
 public:
  static constexpr char const* service_full_name() {
    return "GRPCTestApi.GRPCTestApiSrv";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status GetTime(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::GRPCTestApi::GetTimeResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetTimeResponse>> AsyncGetTime(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetTimeResponse>>(AsyncGetTimeRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetTimeResponse>> PrepareAsyncGetTime(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetTimeResponse>>(PrepareAsyncGetTimeRaw(context, request, cq));
    }
    virtual ::grpc::Status GetServerName(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::GRPCTestApi::GetServerNameResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetServerNameResponse>> AsyncGetServerName(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetServerNameResponse>>(AsyncGetServerNameRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetServerNameResponse>> PrepareAsyncGetServerName(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetServerNameResponse>>(PrepareAsyncGetServerNameRaw(context, request, cq));
    }
    virtual ::grpc::Status Convert(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::GRPCTestApi::ConvertResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::ConvertResponse>> AsyncConvert(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::ConvertResponse>>(AsyncConvertRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::ConvertResponse>> PrepareAsyncConvert(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::ConvertResponse>>(PrepareAsyncConvertRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetTimeResponse>* AsyncGetTimeRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetTimeResponse>* PrepareAsyncGetTimeRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetServerNameResponse>* AsyncGetServerNameRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::GetServerNameResponse>* PrepareAsyncGetServerNameRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::ConvertResponse>* AsyncConvertRaw(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::GRPCTestApi::ConvertResponse>* PrepareAsyncConvertRaw(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status GetTime(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::GRPCTestApi::GetTimeResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetTimeResponse>> AsyncGetTime(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetTimeResponse>>(AsyncGetTimeRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetTimeResponse>> PrepareAsyncGetTime(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetTimeResponse>>(PrepareAsyncGetTimeRaw(context, request, cq));
    }
    ::grpc::Status GetServerName(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::GRPCTestApi::GetServerNameResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetServerNameResponse>> AsyncGetServerName(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetServerNameResponse>>(AsyncGetServerNameRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetServerNameResponse>> PrepareAsyncGetServerName(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetServerNameResponse>>(PrepareAsyncGetServerNameRaw(context, request, cq));
    }
    ::grpc::Status Convert(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::GRPCTestApi::ConvertResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::ConvertResponse>> AsyncConvert(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::ConvertResponse>>(AsyncConvertRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::ConvertResponse>> PrepareAsyncConvert(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::ConvertResponse>>(PrepareAsyncConvertRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetTimeResponse>* AsyncGetTimeRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetTimeResponse>* PrepareAsyncGetTimeRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetTimeRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetServerNameResponse>* AsyncGetServerNameRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::GetServerNameResponse>* PrepareAsyncGetServerNameRaw(::grpc::ClientContext* context, const ::GRPCTestApi::GetServerNameRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::ConvertResponse>* AsyncConvertRaw(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::GRPCTestApi::ConvertResponse>* PrepareAsyncConvertRaw(::grpc::ClientContext* context, const ::GRPCTestApi::ConvertRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_GetTime_;
    const ::grpc::internal::RpcMethod rpcmethod_GetServerName_;
    const ::grpc::internal::RpcMethod rpcmethod_Convert_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status GetTime(::grpc::ServerContext* context, const ::GRPCTestApi::GetTimeRequest* request, ::GRPCTestApi::GetTimeResponse* response);
    virtual ::grpc::Status GetServerName(::grpc::ServerContext* context, const ::GRPCTestApi::GetServerNameRequest* request, ::GRPCTestApi::GetServerNameResponse* response);
    virtual ::grpc::Status Convert(::grpc::ServerContext* context, const ::GRPCTestApi::ConvertRequest* request, ::GRPCTestApi::ConvertResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_GetTime : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetTime() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_GetTime() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTime(::grpc::ServerContext* context, const ::GRPCTestApi::GetTimeRequest* request, ::GRPCTestApi::GetTimeResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetTime(::grpc::ServerContext* context, ::GRPCTestApi::GetTimeRequest* request, ::grpc::ServerAsyncResponseWriter< ::GRPCTestApi::GetTimeResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetServerName : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetServerName() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_GetServerName() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetServerName(::grpc::ServerContext* context, const ::GRPCTestApi::GetServerNameRequest* request, ::GRPCTestApi::GetServerNameResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetServerName(::grpc::ServerContext* context, ::GRPCTestApi::GetServerNameRequest* request, ::grpc::ServerAsyncResponseWriter< ::GRPCTestApi::GetServerNameResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Convert : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_Convert() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_Convert() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Convert(::grpc::ServerContext* context, const ::GRPCTestApi::ConvertRequest* request, ::GRPCTestApi::ConvertResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestConvert(::grpc::ServerContext* context, ::GRPCTestApi::ConvertRequest* request, ::grpc::ServerAsyncResponseWriter< ::GRPCTestApi::ConvertResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_GetTime<WithAsyncMethod_GetServerName<WithAsyncMethod_Convert<Service > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_GetTime : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetTime() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_GetTime() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetTime(::grpc::ServerContext* context, const ::GRPCTestApi::GetTimeRequest* request, ::GRPCTestApi::GetTimeResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetServerName : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetServerName() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_GetServerName() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetServerName(::grpc::ServerContext* context, const ::GRPCTestApi::GetServerNameRequest* request, ::GRPCTestApi::GetServerNameResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Convert : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_Convert() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_Convert() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Convert(::grpc::ServerContext* context, const ::GRPCTestApi::ConvertRequest* request, ::GRPCTestApi::ConvertResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetTime : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_GetTime() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler< ::GRPCTestApi::GetTimeRequest, ::GRPCTestApi::GetTimeResponse>(std::bind(&WithStreamedUnaryMethod_GetTime<BaseClass>::StreamedGetTime, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_GetTime() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetTime(::grpc::ServerContext* context, const ::GRPCTestApi::GetTimeRequest* request, ::GRPCTestApi::GetTimeResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetTime(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::GRPCTestApi::GetTimeRequest,::GRPCTestApi::GetTimeResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetServerName : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_GetServerName() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::StreamedUnaryHandler< ::GRPCTestApi::GetServerNameRequest, ::GRPCTestApi::GetServerNameResponse>(std::bind(&WithStreamedUnaryMethod_GetServerName<BaseClass>::StreamedGetServerName, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_GetServerName() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetServerName(::grpc::ServerContext* context, const ::GRPCTestApi::GetServerNameRequest* request, ::GRPCTestApi::GetServerNameResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetServerName(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::GRPCTestApi::GetServerNameRequest,::GRPCTestApi::GetServerNameResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Convert : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_Convert() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::internal::StreamedUnaryHandler< ::GRPCTestApi::ConvertRequest, ::GRPCTestApi::ConvertResponse>(std::bind(&WithStreamedUnaryMethod_Convert<BaseClass>::StreamedConvert, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_Convert() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Convert(::grpc::ServerContext* context, const ::GRPCTestApi::ConvertRequest* request, ::GRPCTestApi::ConvertResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedConvert(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::GRPCTestApi::ConvertRequest,::GRPCTestApi::ConvertResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_GetTime<WithStreamedUnaryMethod_GetServerName<WithStreamedUnaryMethod_Convert<Service > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_GetTime<WithStreamedUnaryMethod_GetServerName<WithStreamedUnaryMethod_Convert<Service > > > StreamedService;
};

}  // namespace GRPCTestApi


#endif  // GRPC_grpctest_2eproto__INCLUDED