
#include "jitsignature.h"

#include "jittype.h"

JITSignature::JITSignature():
   mTypes()
{
}

void JITSignature::add(JITType* ptype)
{
   mTypes.push_back(ptype);
}

JITSignature* JITSignature::clone() const
{
   JITSignature* presult = new JITSignature(*this);

   for ( int index = 0; index < mTypes.size(); ++index )
   {
      const JITType* ptype = mTypes[index];
      presult->add(ptype->clone());
   }

   return presult;
}
