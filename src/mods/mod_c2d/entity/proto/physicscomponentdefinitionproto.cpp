
#include "physicscomponentdefinitionproto.h"

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

   mDefinition.setMass(mass);
   mDefinition.setStatic(isstatic);
   mDefinition.setFixedRotation(isfixedrot);

   switch ( shapetype )
   {
   case BodyDefinition::eBox:
      {
         float width, height;
         stream.readFloat(width);
         stream.readFloat(height);
         mDefinition.createBox(width, height);
      }
      break;
   case BodyDefinition::eCircle:
      {
         float radius;
         stream.readFloat(radius);
         mDefinition.createCircle(radius);
      }
      break;
   }
}

void PhysicsComponentDefinitionProto::virWrite(DataStream& stream) const
{
   stream.writeInt((int)mDefinition.getShapeType());
   stream.writeFloat(mDefinition.getMass());
   stream.writeBool(mDefinition.isStatic());
   stream.writeBool(mDefinition.isFixedRotation());

   switch ( mDefinition.getShapeType() )
   {
   case BodyDefinition::eBox:
      stream.writeFloat(mDefinition.getWidth());
      stream.writeFloat(mDefinition.getHeight());
      break;
   case BodyDefinition::eCircle:
      stream.writeFloat(mDefinition.getRadius());
      break;
   }
}
