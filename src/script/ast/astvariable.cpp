
#include "astvariable.h"

#include "astvariableinit.h"
#include "asttype.h"

ASTVariable::ASTVariable():
   mName(),
   mModifiers(),
   mpType(NULL),
   mpInit(NULL),
   mResourceIndex(-1)
{
}

ASTVariable::~ASTVariable()
{
   setType(NULL);
   setInit(NULL);
}

// - Get/set

const String& ASTVariable::getName() const
{
   return mName;
}

void ASTVariable::setName(const String& name)
{
   mName = name;
}

const ASTType& ASTVariable::getType() const
{
   return *mpType;
}

ASTType& ASTVariable::getType()
{
   return *mpType;
}

void ASTVariable::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
}

const ASTModifiers& ASTVariable::getModifiers() const
{
   return mModifiers;
}

ASTModifiers& ASTVariable::getModifiers()
{
   return mModifiers;
}

void ASTVariable::setModifiers(const ASTModifiers& modifiers)
{
   mModifiers = modifiers;
}

bool ASTVariable::hasInit() const
{
   return mpInit != NULL;
}

const ASTVariableInit& ASTVariable::getInit() const
{
   return *mpInit;
}

ASTVariableInit& ASTVariable::getInit()
{
   return *mpInit;
}

void ASTVariable::setInit(ASTVariableInit* pinit)
{
   delete mpInit;
   mpInit = pinit;
}

int ASTVariable::getResourceIndex() const
{
   return mResourceIndex;
}

void ASTVariable::setResourceIndex(int index)
{
   mResourceIndex = index;
}
