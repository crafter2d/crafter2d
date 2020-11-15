
#include "astcast.h"

#include "asttype.h"
#include "astvisitor.h"

ASTCast::ASTCast():
   ASTExpressionPart(),
   mpType(nullptr),
   mpNode(nullptr)
{
}

ASTCast::~ASTCast()
{
   setType(nullptr);
   setNode(nullptr);
}

// - Get/set

const ASTType& ASTCast::getType() const
{
   return *mpType;
}

ASTType& ASTCast::getType()
{
   return *mpType;
}
   
void ASTCast::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
}

const ASTNode& ASTCast::getNode() const
{
   return *mpNode;
}

ASTNode& ASTCast::getNode()
{
   return *mpNode;
}

void ASTCast::setNode(ASTNode* pnode)
{
   delete mpNode;
   mpNode = pnode;
}

// - Visitor

ACCEPT_IMPL(ASTCast);
