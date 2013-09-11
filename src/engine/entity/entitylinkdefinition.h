
#ifndef ENTITY_LINK_DEFINITION_H
#define ENTITY_LINK_DEFINITION_H

#include "engine/physics/simulator.h"

class JointDefinition;

class EntityLinkDefinition
{
public:
   EntityLinkDefinition(int left, int right, JointDefinition* pjoint);

 // get/set
   int getLeftIndex() const;
   int getRightIndex() const;

   JointDefinition& getJointDefinition() const;

private:

 // data
   int               mLeftIndex;
   int               mRightIndex;
   JointDefinition*  mpJoint;
};

#endif // ENTITY_LINK_DEFINITION_H
