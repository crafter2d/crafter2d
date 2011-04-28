
#include "astvariable.h"

#include "astexpression.h"
#include "asttype.h"

ASTVariable::ASTVariable():
   mName(),
   mModifiers(),
   mpType(NULL),
   mpExpression(NULL),
   mResourceIndex(-1)
{
}

ASTVariable::~ASTVariable()
{
   setType(NULL);
   setExpression(NULL);
}

// - Get/set

const std::string& ASTVariable::getName() const
{
   return mName;
}

void ASTVariable::setName(const std::string& name)
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

bool ASTVariable::hasExpression() const
{
   return mpExpression != NULL;
}

const ASTExpression& ASTVariable::getExpression() const
{
   return *mpExpression;
}

ASTExpression& ASTVariable::getExpression()
{
   return *mpExpression;
}

void ASTVariable::setExpression(ASTExpression* pexpression)
{
   delete mpExpression;
   mpExpression = pexpression;
}

int ASTVariable::getResourceIndex() const
{
   return mResourceIndex;
}

void ASTVariable::setResourceIndex(int index)
{
   mResourceIndex = index;
}
