
#ifndef LINK_DEFINITION_PROTO_H
#define LINK_DEFINITION_PROTO_H

#include "core/string/string.h"

class DataStream;

class LinkDefinitionProto
{
public:
   enum Type { eRevolute };

   static Type fromString(const String& type);

   void read(DataStream& stream);
   void write(DataStream& stream) const;

   Type   mType;
   String mLeft;
   String mRight;
};

#endif // LINK_DEFINITION_PROTO_H
