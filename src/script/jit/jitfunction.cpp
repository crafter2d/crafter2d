
#include "jitfunction.h"

#include "jitcontext.h"
#include "jitsignature.h"

// static
JITFunction* JITFunction::create(JITContext& context, const JITSignature& signature)
{
   JITFunction* pfunction = new JITFunction(context);
   pfunction->setSignature(signature);

   context.add(pfunction);

   return pfunction;
}

JITFunction::JITFunction(JITContext& context):
   mContext(context)
{
}

// - Get/set

void JITFunction::setSignature(const JITSignature& signature)
{
   mpSignature = signature.clone();
}
