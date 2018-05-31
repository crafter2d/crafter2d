
#include "netobjectstream.h"

#include "events/netevent.h"
#include "netobject.h"
#include "netobjectfactory.h"
#include "neteventfactory.h"

NetObjectStream::NetObjectStream(DataStream& stream):
   NetStream(stream)
{
}

// - Streaming

NetObjectStream& NetObjectStream::operator<<(const NetObject& object)
{
   writeInt(object.getRuntimeInfo().getId());
   object.pack(*this);
   return *this;
}

NetObjectStream& NetObjectStream::operator>>(NetObject** obj)
{
   int type;
   readInt(type);

   (*obj) = NetObjectFactory::getInstance().createObject(type);
   ASSERT_PTR((*obj));
   (*obj)->setReplica();
   (*obj)->unpack(*this);
   return *this;
}

NetEvent* NetObjectStream::readEvent()
{
   int type;
   readInt(type);

   NetEvent* pevent = NetEventFactory::getInstance().create(type);
   if ( pevent == nullptr )
   {
      pevent = static_cast<NetEvent*>(NetObjectFactory::getInstance().createObject(type));
   }
   pevent->setReplica();
   pevent->unpack(*this);

   return pevent;
}
