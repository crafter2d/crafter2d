
#include "entitylinkdefinition.h"

#include "core/defines.h"

EntityLinkDefinition::EntityLinkDefinition(int left, int right, JointDefinition* pdefinition):
   mLeftIndex(left),
   mRightIndex(right),
   mpJoint(pdefinition)
{
}

// - Get/set

int EntityLinkDefinition::getLeftIndex() const
{
   return mLeftIndex;
}

int EntityLinkDefinition::getRightIndex() const
{
   return mRightIndex;
}

JointDefinition& EntityLinkDefinition::getJointDefinition() const
{
   ASSERT_PTR(mpJoint);
   return *mpJoint;
}
