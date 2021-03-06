#include "nan.h"

#include "util.h"
#include "SignalHandlerImpl.h"
#include <Message.h>
#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/AllJoynStd.h>

SignalHandlerImpl::SignalHandlerImpl(NanCallback* sig){
  loop = uv_default_loop();
  signalCallback.callback = sig;
  uv_async_init(loop, &signal_async, signal_callback);
}

SignalHandlerImpl::~SignalHandlerImpl(){
}

void SignalHandlerImpl::signal_callback(uv_async_t *handle, int status) {
    CallbackHolder* holder = (CallbackHolder*) handle->data;

    v8::Local<v8::Object> msg = v8::Object::New();
    size_t msgIndex = 0;
    const ajn::MsgArg* arg = (*holder->message)->GetArg(msgIndex);
    while(arg != NULL){
      msgArgToObject(arg, msgIndex, msg);
      msgIndex++;
      arg = (*holder->message)->GetArg(msgIndex);
    }

    v8::Local<v8::Object> sender = v8::Object::New();
    sender->Set(NanNew<v8::String>("sender"), NanNew<v8::String>((*holder->message)->GetSender()));
    sender->Set(NanNew<v8::String>("session_id"), NanNew<v8::Integer>((*holder->message)->GetSessionId()));
    sender->Set(NanNew<v8::String>("timestamp"), NanNew<v8::Integer>((*holder->message)->GetTimeStamp()));
    sender->Set(NanNew<v8::String>("member_name"), NanNew<v8::String>((*holder->message)->GetMemberName()));
    sender->Set(NanNew<v8::String>("object_path"), NanNew<v8::String>((*holder->message)->GetObjectPath()));
    sender->Set(NanNew<v8::String>("signature"), NanNew<v8::String>((*holder->message)->GetSignature()));

    v8::Handle<v8::Value> argv[] = {
      msg,
      sender
    };
    holder->callback->Call(2, argv);

    if(holder->message){
      delete holder->message;
      holder->message = NULL;
    }
}

void SignalHandlerImpl::Signal(const ajn::InterfaceDescription::Member *member, const char *srcPath, ajn::Message &message){
    signal_async.data = (void*) &signalCallback;
    signalCallback.message = new ajn::Message(message);
    uv_async_send(&signal_async);
}
