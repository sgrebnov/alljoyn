#ifndef LD_METHODHANDLERIMPL_H
#define LD_METHODHANDLERIMPL_H

#include <nan.h>
#include <uv.h>
#include <alljoyn/BusObject.h>
#include <InterfaceDescription.h>
#include <TransportMask.h>
#include <alljoyn/AllJoynStd.h>
#include <alljoyn/MessageReceiver.h>
#include <Message.h>

class MethodHandlerImpl: public ajn::MessageReceiver {
  private:
  	// uv_loop_t *loop;
  	// uv_async_t signal_async;

  // struct CallbackHolder{
  //   NanCallback* callback;
  //   ajn::Message* message;
  // } signalCallback;

  public:
  	MethodHandlerImpl(NanCallback* sig);
  	~MethodHandlerImpl();
  	static void signal_callback(uv_async_t *handle, int status);

    void Call(const ajn::InterfaceDescription::Member *member, ajn::Message &message);
};

#endif