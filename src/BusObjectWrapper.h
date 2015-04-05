#ifndef LD_BUSOBJECTWRAPPER_H
#define LD_BUSOBJECTWRAPPER_H

#include <nan.h>
#include <alljoyn/BusObject.h>
#include <alljoyn/AllJoynStd.h>

NAN_METHOD(BusObjectConstructor);

class BusObjectImpl : public ajn::BusObject{
public:
    BusObjectImpl(const char* path);
    ~BusObjectImpl();
    QStatus AddInter(ajn::InterfaceDescription* interface);
    QStatus MethodReplyWrapper(const ajn::Message &msg, const ajn::MsgArg *args, size_t numArgs);
    QStatus SetMethodHandler(const ajn::InterfaceDescription::Member* member, NanCallback* callback);
    void CallbackMethod(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);
};

class BusObjectWrapper : public node::ObjectWrap {
  private:

    static NAN_METHOD(New);
    static NAN_METHOD(AddInterfaceInternal);
    static NAN_METHOD(Signal);
    static NAN_METHOD(AddMethodHandlerInternal);
    static NAN_METHOD(Reply);
  public:
  	BusObjectWrapper(const char* path);
    static void Init ();
    static v8::Handle<v8::Value> NewInstance();
    BusObjectImpl* object;
};


#endif
