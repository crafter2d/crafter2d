
#include "physicscomponentdefinitionproto.h"

#include <stdexcept>

#include "core/defines.h"
#include "core/entity/components/componentinterface.h"
#include "core/streams/datastream.h"


PhysicsComponentDefinitionProto::PhysicsComponentDefinitionProto():
   ComponentDefinitionProto(ComponentInterface::ePhysisComponent),
   mDefinition()
{
}

// - Storage

void PhysicsComponentDefinitionProto::virRead(DataStream& stream)
{
   BodyDefinition::ShapeType shapetype;
   bool isstatic, isfixedrot;
   float mass;

   stream.readInt((int&)shapetype);
   stream.readFloat(mass);
   stream.readBool(isstatic);
   stream.readBool(isfixedrot);
   
   if ( shapetype == BodyDefinition::eInvalidType )
   {
      throw std::runtime_error("Invalid physics body type.");
   }

   mDefinition.setMass(mass);
   mDefinition.setStatic(isstatic);
   mDefinition.setFixedRotation(isfixedrot);

   if ( shapetype == BodyDefinition::eBox )
   {
      float width, height;
      stream.readFloat(width);
      stream.readFloat(height);
      mDefinition.createBox(width, height);
   }
   else
   {
      ASSERT(shapetype == BodyDefinition::eCircle);
      float radius;
      stream.readFloat(radius);
      mDefinition.createCircle(radius);
   }
}

void PhysicsComponentDefinitionProto::virWrite(DataStream& stream) const
{
   stream.writeInt((int)mDefinition.getShapeType());
   stream.writeFloat(mDefinition.getMass());
   stream.writeBool(mDefinition.isStatic());
   stream.writeBool(mDefinition.isFixedRotation());

   if ( mDefinition.getShapeType() )
   {
      stream.writeFloat(mDefinition.getWidth());
      stream.writeFloat(mDefinition.getHeight());
   }
   else
   {
      ASSERT(mDefinition.getShapeType() == BodyDefinition::eCircle);
      stream.writeFloat(mDefinition.getRadius());
   }
}
