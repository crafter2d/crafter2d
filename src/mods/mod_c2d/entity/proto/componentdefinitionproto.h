
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
   virtual ~ComponentDefinitionProto();

 // get/set
   ComponentInterface::ComponentType getType() const;

 // storage
   void read(DataStream& stream);
   void write(DataStream& stream) const;

protected:
 // storage
   virtual void virRead(DataStream& stream) = 0;
   virtual void virWrite(DataStream& stream) const = 0;

private:

 // data
   ComponentInterface::ComponentType mType;
};

#endif // COMPONENT_DEFINITION_PROTO_H
