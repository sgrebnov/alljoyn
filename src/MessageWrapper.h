#ifndef LD_MESSAGERAPPER_H
#define LD_MESSAGERAPPER_H

#include <nan.h>
#include <alljoyn/Message.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/AllJoynStd.h>

// http://pagesofinterest.net/blog/2013/03/constructing-js-objects-in-cpp/

NAN_METHOD(MessageDescriptionWrapper);

class MessageWrapper : public node::ObjectWrap {
  private:
    static NAN_METHOD(New);
  public:
  	MessageWrapper();
    MessageWrapper(ajn::Message* msg);
    ajn::Message* message;
    static void Init ();
    static v8::Handle<v8::Object> NewInstance();
};

#endif