
#include "componentdefinitionproto.h"

#include "core/entity/components/componentinterface.h"
#include "core/streams/datastream.h"
#include "core/defines.h"

#include "meshcomponentdefinitionproto.h"
#include "physicscomponentdefinitionproto.h"

// static 
ComponentDefinitionProto* ComponentDefinitionProto::fromStream(DataStream& stream)
{
   ComponentInterface::ComponentType type;
   stream.readInt((int&) type);

   ComponentDefinitionProto* presult = NULL;
   switch ( type )
   {
   case ComponentInterface::eMeshComponent:
      presult = new MeshComponentDefinitionProto();
      break;
   case ComponentInterface::ePhysisComponent:
      presult = new PhysicsComponentDefinitionProto();
      break;
   default:
      throw std::exception("Invalid component type.");
   }

   presult->read(stream);
   return presult;
}

ComponentDefinitionProto::ComponentDefinitionProto(ComponentInterface::ComponentType type):
   mType(type)
{
}

// - Get/set

ComponentInterface::ComponentType ComponentDefinitionProto::getType() const
{
   return mType;
}
