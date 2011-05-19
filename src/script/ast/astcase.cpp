
#include "astcase.h"

#include "core/defines.h"

#include "astvisitor.h"

ASTCase::ASTCase():
   ASTNode(),
   mKind(eCase),
   mpType(NULL),
   mpBody(NULL),
   mValue()
{
}

ASTCase::~ASTCase()
{
   delete mpType;

   setBody(NULL);
}

// - Get/set

void ASTCase::setKind(Kind kind)
{
   mKind = kind;
}

const Variant& ASTCase::getValue() const
{
   return mValue;
}
   
void ASTCase::setValue(const Variant& value)
{
   mValue = value;

   if ( mValue.isInt() )
   {
      setType(new ASTType(ASTType::eInt));
   }
   else if ( mValue.isReal() )
   {
      setType(new ASTType(ASTType::eReal));
   }
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
