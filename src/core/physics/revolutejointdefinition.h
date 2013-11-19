
#ifndef REVOLUTE_JOINT_DEFINITION_H
#define REVOLUTE_JOINT_DEFINITION_H

#include "core/math/vector.h"
#include "core/core_base.h"

#include "jointdefinition.h"

class CORE_API RevoluteJointDefinition : public JointDefinition
{
public:
   RevoluteJointDefinition();

   Vector anchor;
};

#endif // REVOLUTE_JOINT_DEFINITION_H
