
#include "asttypevariable.h"

#include "core/defines.h"

#include "asttype.h"

ASTTypeVariable::ASTTypeVariable():
   mId(),
   mMode(eNormal),
   mpOther(NULL),
   mIndex(-1)
{
}

ASTTypeVariable::~ASTTypeVariable()
{
   setOther(NULL);
}

// get/set

const std::string& ASTTypeVariable::getIdentifier() const
{
   return mId;
}

void ASTTypeVariable::setIdentifier(const std::string& id)
{
   mId = id; 
}

ASTTypeVariable::Mode ASTTypeVariable::getMode() const
{
   return mMode;
}

void ASTTypeVariable::setMode(Mode mode)
{
   mMode = mode;
}

const ASTType& ASTTypeVariable::getOther() const
{
   ASSERT_PTR(mpOther);
   return *mpOther;
}

void ASTTypeVariable::setOther(ASTType* ptype)
{
   delete mpOther;
   mpOther = ptype;
}

int ASTTypeVariable::getIndex() const
{
   ASSERT(mIndex >= 0);
   return mIndex;
}

void ASTTypeVariable::setIndex(int index)
{
   mIndex = index;
}
