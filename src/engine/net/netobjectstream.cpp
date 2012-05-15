
#include "netobjectstream.h"

#include "netobject.h"
#include "netobjectfactory.h"

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
