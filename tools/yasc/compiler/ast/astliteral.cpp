
#include "astliteral.h"

#include "core/defines.h"

#include "yasc/common/literal.h"

#include "astvisitor.h"

ASTLiteral::ASTLiteral(const Literal& literal, ASTType::Kind kind):
   ASTExpressionPart(),
   mLiteral(literal),
   mpType(new ASTType(kind))
{
   ASSERT(literal.getTableIndex() >= 0);
}

ASTLiteral::~ASTLiteral()
{
   delete mpType;
}

// - Get/set

const ASTType& ASTLiteral::getType() const
{
   return *mpType;
}

const Literal& ASTLiteral::getLiteral() const
{
   return mLiteral;
}

// - Visitor
   
ACCEPT_IMPL(ASTLiteral)
