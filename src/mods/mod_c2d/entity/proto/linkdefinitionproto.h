
#ifndef LINK_DEFINITION_PROTO_H
#define LINK_DEFINITION_PROTO_H

#include "core/physics/jointdefinition.h"
#include "core/string/string.h"

class DataStream;

class LinkDefinitionProto
{
public:

 // storage
   void read(DataStream& stream);
   void write(DataStream& stream) const;

 // data
   JointDefinition* mpJointDef;
   int  mLeft;
   int  mRight;
};

#endif // LINK_DEFINITION_PROTO_H
