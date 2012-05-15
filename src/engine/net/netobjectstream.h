
#ifndef NET_OBJECT_STREAM_H
#define NET_OBJECT_STREAM_H

#include "netstream.h"

class NetObject;

class NetObjectStream : public NetStream
{
public:
   explicit NetObjectStream(DataStream& stream);

 // streaming
   NetObjectStream&  operator<<(const NetObject& object);
   NetObjectStream&  operator>>(NetObject** obj);
};

#endif // NET_OBJECT_STREAM_H
