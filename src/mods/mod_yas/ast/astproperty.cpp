
#include "astproperty.h"

#include "asttype.h"
#include "astvisitor.h"

ASTProperty::ASTProperty():
   ASTMember(ASTMember::eProperty),
   mName(),
   mpType(NULL)
{
}

ASTProperty::~ASTProperty()
{
   setType(NULL);
}

// - Get/set

const String& ASTProperty::getName() const
{
   return mName;
}

void ASTProperty::setName(const String& name)
{
   mName = name;
}

const ASTType& ASTProperty::getType() const
{
   ASSERT_PTR(mpType);
   return *mpType;
}

ASTType& ASTProperty::getType()
{
   ASSERT_PTR(mpType);
   return *mpType;
}

void ASTProperty::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
}

// - Visitor

ACCEPT_IMPL(ASTProperty);
