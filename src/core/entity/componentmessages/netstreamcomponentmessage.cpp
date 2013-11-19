
#include "netstreamcomponentmessage.h"

#include "core/entity/components/componentinterface.h"

NetStreamComponentMessage::NetStreamComponentMessage(DataStream& stream, bool write):
   ComponentMessage(ComponentInterface::eNetStreamMsg, NULL),
   mStream(stream),
   mWrite(write)
{
}

bool NetStreamComponentMessage::isWrite() const
{
   return mWrite;
}

DataStream& NetStreamComponentMessage::getStream()
{
   return mStream;
}
