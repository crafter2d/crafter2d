
#include "linkdefinitionproto.h"

#include "core/physics/revolutejointdefinition.h"

#include "core/streams/datastream.h"

void LinkDefinitionProto::read(DataStream& stream)
{
   JointDefinition::Kind kind;
   stream.readInt((int&) kind);

   switch ( kind )
   {
   case JointDefinition::eRevolute:
      {
         RevoluteJointDefinition* prev = static_cast<RevoluteJointDefinition*>(mpJointDef);
         stream.readFloat(prev->anchor.x);
         stream.readFloat(prev->anchor.y);

         mpJointDef = prev;
      }
      break;
   }

   stream >> mLeft >> mRight;
}

void LinkDefinitionProto::write(DataStream& stream) const
{
   JointDefinition::Kind kind = mpJointDef->getKind();
   stream.writeInt((int) kind);

   switch ( kind )
   {
   case JointDefinition::eRevolute:
      {
         RevoluteJointDefinition* prev = static_cast<RevoluteJointDefinition*>(mpJointDef);
         stream.writeFloat(prev->anchor.x);
         stream.writeFloat(prev->anchor.y);
      }
      break;
   }

   stream << mLeft << mRight;
}
