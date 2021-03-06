
#include "astunary.h"

#include "core/defines.h"

#include "astaccess.h"
#include "astvisitor.h"
#include "astexpressionpart.h"

ASTUnary::ASTUnary():
   ASTNode(),
   mPre(eNone),
   mPost(eNone),
   mpAccess(nullptr)
{
}

// - Get/set

ASTUnary::Operator ASTUnary::getPre() const
{
   return mPre;
}

void ASTUnary::setPre(Operator op)
{
   mPre = op;
}

ASTUnary::Operator ASTUnary::getPost() const
{
   return mPost;
}

void ASTUnary::setPost(Operator op)
{
   mPost = op;
}

// - Query

const ASTAccess* ASTUnary::asVariable() const
{
   return dynamic_cast<const ASTAccess*>(&getChildren()[0]);
}

// - Operations

void ASTUnary::addPart(ASTExpressionPart* ppart)
{
   ASTAccess* paccess = dynamic_cast<ASTAccess*>(ppart);

   if ( mpAccess != nullptr )
   {
      ASSERT_PTR(paccess);

      mpAccess->setNext(*paccess);
      paccess->setPrevious(*mpAccess);

      mpAccess = paccess;
   }
   else if ( getChildren().isEmpty() && paccess != nullptr )
   {
      mpAccess = paccess;
   }

   addChild(ppart);
}

const ASTNodes& ASTUnary::getParts() const
{
   return getChildren();
}

ASTNodes& ASTUnary::getParts()
{
   return getChildren();
}

// - Visitor

ACCEPT_IMPL(ASTUnary)
