#include "nan.h"

#include "MessageWrapper.h"
#include <alljoyn/Message.h>
#include <alljoyn/AllJoynStd.h>

static v8::Persistent<v8::FunctionTemplate> message_constructor;

v8::Handle<v8::Object> MessageWrapper::NewInstance() {
    NanScope();

  v8::Local<v8::Object> obj;
  obj = message_constructor->GetFunction()->NewInstance();

  NanReturnValue(obj);
}

NAN_METHOD(MessageDescriptionWrapper) {
    NanScope();
    NanReturnValue(MessageWrapper::NewInstance());
}

MessageWrapper::MessageWrapper(){
  message = NULL;
}

MessageWrapper::MessageWrapper(ajn::Message* msg){
  message = msg;
}

void MessageWrapper::Init () {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(MessageWrapper::New);

  NanAssignPersistent(message_constructor, tpl);
  tpl->SetClassName(NanNew<v8::String>("Message"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
}

NAN_METHOD(MessageWrapper::New) {
  NanScope();
  MessageWrapper* obj = new MessageWrapper();
  obj->Wrap(args.This());

  NanReturnValue(args.This());
}
