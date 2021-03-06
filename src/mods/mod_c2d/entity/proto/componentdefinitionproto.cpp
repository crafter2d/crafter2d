
#include "componentdefinitionproto.h"

#include "core/entity/components/componentinterface.h"
#include "core/streams/datastream.h"
#include "core/defines.h"

#include "meshcomponentdefinitionproto.h"
#include "particlecomponentdefinitionproto.h"
#include "physicscomponentdefinitionproto.h"

// static 
ComponentDefinitionProto* ComponentDefinitionProto::fromStream(DataStream& stream)
{
   ComponentInterface::ComponentType type;
   stream.readInt((int&) type);

   ComponentDefinitionProto* presult = nullptr;
   switch ( type )
   {
   case ComponentInterface::eMeshComponent:
      presult = new MeshComponentDefinitionProto();
      break;
   case ComponentInterface::eParticleComponent:
      presult = new ParticleComponentDefinitionProto();
      break;
   case ComponentInterface::ePhysisComponent:
      presult = new PhysicsComponentDefinitionProto();
      break;
   default:
      throw std::runtime_error("Invalid component type.");
   }

   presult->read(stream);
   return presult;
}

ComponentDefinitionProto::ComponentDefinitionProto(ComponentInterface::ComponentType type):
   mType(type)
{
}

ComponentDefinitionProto::~ComponentDefinitionProto()
{
}

// - Get/set

ComponentInterface::ComponentType ComponentDefinitionProto::getType() const
{
   return mType;
}

// - Storage

void ComponentDefinitionProto::read(DataStream& stream)
{
   virRead(stream);
}

void ComponentDefinitionProto::write(DataStream& stream) const
{
   stream.writeInt(mType);
   virWrite(stream);
}
