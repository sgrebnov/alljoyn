#include "nan.h"

#include "util.h"
#include "MethodHandlerImpl.h"
#include "MessageWrapper.h"
#include <Message.h>
#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/AllJoynStd.h>

uv_loop_t *loop;
uv_async_t signal_async;

  struct CallbackHolder{
    NanCallback* callback;
    ajn::Message* message;
  } signalCallback;

MethodHandlerImpl::MethodHandlerImpl(NanCallback* sig){
  loop = uv_default_loop();
  signalCallback.callback = sig;
  uv_async_init(loop, &signal_async, signal_callback);
}

MethodHandlerImpl::~MethodHandlerImpl(){
}


void MethodHandlerImpl::signal_callback(uv_async_t *handle, int status) {
    CallbackHolder* holder = (CallbackHolder*) handle->data;
    //ajn::Message* msg = holder->message;

    v8::Local<v8::Object> args = v8::Object::New();
    size_t msgIndex = 0;
    const ajn::MsgArg* arg = (*holder->message)->GetArg(msgIndex);
    while(arg != NULL){
      msgArgToObject(arg, msgIndex, args);
      msgIndex++;
      arg = (*holder->message)->GetArg(msgIndex);
    }
    
    v8::Handle<v8::Object> msg = MessageWrapper::NewInstance();
    //msg = MessageWrapper::NewInstance();
     MessageWrapper* messageWrap = node::ObjectWrap::Unwrap<MessageWrapper>(msg);
     messageWrap->message = holder->message;

    v8::Handle<v8::Value> argv[] = {
      args,
      msg
    };
    holder->callback->Call(2, argv);


        //     qcc::String inStr1 = msg->GetArg(0)->v_string.str;
        // qcc::String outStr = inStr1 + " result";

        // ajn::MsgArg outArg("s", outStr.c_str());
        // QStatus status = MethodReply(msg, &outArg, 1);

    // if(holder->message){
    //   delete holder->message;
    //   holder->message = NULL;
    // }
}

void MethodHandlerImpl::Call(const ajn::InterfaceDescription::Member *member, ajn::Message &message){
    signal_async.data = (void*) &signalCallback;
    signalCallback.message = new ajn::Message(message);
    uv_async_send(&signal_async);
}
