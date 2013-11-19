
#include "physicscomponentdefinitionproto.h"

#include "core/entity/components/componentinterface.h"
#include "core/streams/datastream.h"


PhysicsComponentDefinitionProto::PhysicsComponentDefinitionProto():
   ComponentDefinitionProto(ComponentInterface::ePhysisComponent),
   mShape(eInvalid),
   mInfoA(0.0f),
   mInfoB(0.0f),
   mStatic(false),
   mFixedRotation(false)
{
}

// - Storage

void PhysicsComponentDefinitionProto::read(DataStream& stream)
{
   stream >> (int&)mShape >> mInfoA >> mInfoB >> mStatic >> mFixedRotation;
}

void PhysicsComponentDefinitionProto::write(DataStream& stream) const
{
   stream.writeInt(ComponentInterface::ePhysisComponent);
   stream << (int)mShape << mInfoA << mInfoB << mStatic << mFixedRotation;
}
