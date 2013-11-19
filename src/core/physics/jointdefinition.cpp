
#include "jointdefinition.h"

JointDefinition::JointDefinition(Kind kind):
   mKind(kind)
{
}

// - Get/set

JointDefinition::Kind JointDefinition::getKind() const
{
   return mKind;
}
