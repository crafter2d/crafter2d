
#ifndef CHILD_DEFINITION_PROTO_H
#define CHILD_DEFINITION_PROTO_H

#include "core/math/vector.h"
#include "core/string/string.h"

class ComponentFactories;
class DataStream;
class Entity;

class ChildDefinitionProto
{
public:
   enum RefType { eLocalReference, eFileReference };

   void read(DataStream& stream);
   void write(DataStream& stream) const;

   String      mID;
   RefType     mRefType;
   String      mRef;
   Vector      mOffset;
};

#endif // CHILD_DEFINITION_PROTO_H
