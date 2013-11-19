
#ifndef REVOLUTE_JOINT_DEFINITION_H
#define REVOLUTE_JOINT_DEFINITION_H

#include "core/math/vector.h"

#include "jointdefinition.h"

class RevoluteJointDefinition : public JointDefinition
{
public:
   RevoluteJointDefinition();

   Vector anchor;
};

#endif // REVOLUTE_JOINT_DEFINITION_H
