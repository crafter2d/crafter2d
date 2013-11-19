
#include "linkdefinitionproto.h"

#include "core/streams/datastream.h"


// static 
LinkDefinitionProto::Type LinkDefinitionProto::fromString(const String& type)
{
   LinkDefinitionProto::Type result;
   if ( type == UTEXT("revolute") )
   {
      result = LinkDefinitionProto::eRevolute;
   }

   return result;
}

void LinkDefinitionProto::read(DataStream& stream)
{
   stream >> (int&) mType >> mLeft >> mRight;
}

void LinkDefinitionProto::write(DataStream& stream) const
{
   stream << (int) mType << mLeft << mRight;
}
