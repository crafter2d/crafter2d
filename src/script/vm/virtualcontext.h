
#ifndef VIRTUAL_CONTEXT_H_
#define VIRTUAL_CONTEXT_H_

#include "script/script_base.h"

#include "script/cil/classmap.h"
#include "script/common/classregistry.h"
#include "script/common/stringcache.h"
#include "script/common/literaltable.h"

#include "virtualclasstable.h"
#include "virtualinstructiontable.h"

class SCRIPT_API VirtualContext
{
public:
   VirtualContext();

   VirtualClassTable       mClassTable;
   VirtualInstructionTable mInstructions;
   CIL::ClassMap           mCilClasses;
   ClassRegistry           mNativeRegistry;
   StringCache             mStringCache;
   LiteralTable            mLiteralTable;
};

#endif // VIRTUAL_CONTEXT_H_
