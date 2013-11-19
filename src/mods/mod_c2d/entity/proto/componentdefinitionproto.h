
#ifndef COMPONENT_DEFINITION_PROTO_H
#define COMPONENT_DEFINITION_PROTO_H

#include "core/entity/components/componentinterface.h"

class DataStream;

class ComponentDefinitionProto
{
public:
 // statics
   static ComponentDefinitionProto* fromStream(DataStream& stream);

   explicit ComponentDefinitionProto(ComponentInterface::ComponentType type);

 // get/set
   ComponentInterface::ComponentType getType() const;

 // storage
   virtual void read(DataStream& stream) = 0;
   virtual void write(DataStream& stream) const = 0;

private:

 // data
   ComponentInterface::ComponentType mType;
};

#endif // COMPONENT_DEFINITION_PROTO_H
