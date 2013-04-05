
#include "codepatch.h"

#include "core/defines.h"

CodePatch::CodePatch(Kind kind):
   mKind(kind),
   mOffset(-1),
   mpVirFunction(NULL)
{
}

// - Get/set

CodePatch::Kind CodePatch::getKind() const
{
   return mKind;
}

int CodePatch::getOffset() const
{
   return mOffset;
}

void CodePatch::setOffset(int offset)
{
   mOffset = offset;
}

VirtualFunction& CodePatch::getVirtualFunction()
{
   ASSERT_PTR(mpVirFunction);
   return *mpVirFunction;
}

void CodePatch::setVirtualFunction(VirtualFunction& function)
{
   mpVirFunction = &function;
}
