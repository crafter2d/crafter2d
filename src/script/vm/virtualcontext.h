
#ifndef VIRTUAL_CONTEXT_H_
#define VIRTUAL_CONTEXT_H_

#include "script/common/literaltable.h"

#include "virtualclasstable.h"
#include "virtualinstructiontable.h"

class VirtualContext
{
public:
   VirtualContext();

   VirtualClassTable       mClassTable;
   VirtualInstructionTable mInstructions;
   LiteralTable            mLiteralTable;
};

#endif // VIRTUAL_CONTEXT_H_
