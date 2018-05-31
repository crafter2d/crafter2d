
#include "astvariable.h"

#include "astvariableinit.h"
#include "asttype.h"

ASTVariable::ASTVariable():
   mName(),
   mPosition(),
   mModifiers(),
   mpType(nullptr),
   mpInit(nullptr),
   mLocation(eInvalidLoc),
   mResourceIndex(-1)
{
}

ASTVariable::~ASTVariable()
{
   setType(nullptr);
   setInit(nullptr);
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

const AntlrTextPosition& ASTVariable::getPosition() const
{
   return mPosition;
}

void ASTVariable::setPosition(const AntlrTextPosition& position)
{
   mPosition = position;
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
   return mpInit != nullptr;
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

ASTVariable::Location ASTVariable::getLocation() const
{
   return mLocation;
}

void ASTVariable::setLocation(Location location)
{
   mLocation = location;
}

int ASTVariable::getResourceIndex() const
{
   return mResourceIndex;
}

void ASTVariable::setResourceIndex(int index)
{
   mResourceIndex = index;
}

// query

bool ASTVariable::isArgument() const
{
   ASSERT(mLocation != eInvalidLoc);
   return mLocation == eArgument;
}