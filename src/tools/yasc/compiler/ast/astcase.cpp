
#include "astcase.h"

#include "core/defines.h"

#include "astunary.h"
#include "astvisitor.h"

ASTCase::ASTCase():
   ASTNode(),
   mKind(eCase),
   mpType(nullptr),
   mValue(),
   mpValueExpression(nullptr),
   mpBody(nullptr)
{
}

ASTCase::~ASTCase()
{
   delete mpType;

   setValueExpression(nullptr);
   setBody(nullptr);
}

// - Get/set

void ASTCase::setKind(Kind kind)
{
   mKind = kind;
}

bool ASTCase::hasValue() const
{
   return !mValue.isEmpty();
}

const Variant& ASTCase::getValue() const
{
   return mValue;
}
   
void ASTCase::setValue(const Variant& value)
{
   mValue = value;
}

bool ASTCase::hasValueExpression() const
{
   return mpValueExpression != nullptr;
}

const ASTUnary& ASTCase::getValueExpression() const
{
   ASSERT_PTR(mpValueExpression);
   return *mpValueExpression;
}

ASTUnary& ASTCase::getValueExpression()
{
   ASSERT_PTR(mpValueExpression);
   return *mpValueExpression;
}

void ASTCase::setValueExpression(ASTUnary* pvalueexpr)
{
   delete mpValueExpression;
   mpValueExpression = pvalueexpr;
}

const ASTNode& ASTCase::getBody() const
{
   ASSERT_PTR(mpBody);
   return *mpBody;
}

ASTNode& ASTCase::getBody()
{
   ASSERT_PTR(mpBody);
   return *mpBody;
}

void ASTCase::setBody(ASTNode* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

const ASTType& ASTCase::getType() const
{
   ASSERT_PTR(mpType);
   return *mpType;
}

void ASTCase::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
}

// - Query

bool ASTCase::isCase() const
{
   return mKind == eCase;
}

bool ASTCase::isDefault() const
{
   return mKind == eDefault;
}

// - Visitor

ACCEPT_IMPL(ASTCase);
