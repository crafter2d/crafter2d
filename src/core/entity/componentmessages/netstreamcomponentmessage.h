
#ifndef NET_STREAM_COMPONENT_MESSAGE_H
#define NET_STREAM_COMPONENT_MESSAGE_H

#include "core/core_base.h"

#include "componentmessage.h"

class DataStream;

class CORE_API NetStreamComponentMessage : public ComponentMessage
{
public:
   NetStreamComponentMessage(DataStream& stream, bool write);

 // get/set
   bool isWrite() const;

   DataStream& getStream();

private:

 // data
   DataStream& mStream;
   bool        mWrite;
};

#endif // NET_STREAM_COMPONENT_MESSAGE_H
