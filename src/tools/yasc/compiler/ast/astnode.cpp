
#include "astnode.h"

ASTNode::ASTNode():
   mChildren(),
   mPosition()
{
}

ASTNode::~ASTNode()
{
}

// - Get/set

const AntlrTextPosition& ASTNode::getPosition() const
{
   return mPosition;
}

void ASTNode::setPosition(const AntlrTextPosition& pos)
{
   mPosition = pos;
}

// - Query

const ASTNodes& ASTNode::getChildren() const
{
   return mChildren;
}

// - Children

bool ASTNode::hasChildren() const
{
   return !mChildren.isEmpty();
}

void ASTNode::addChild(ASTNode* pchild)
{
   mChildren.add(pchild);
}

void ASTNode::insertChild(int index, ASTNode* pchild)
{
   mChildren.insert(index, pchild);
}

void ASTNode::removeChild(ASTNode** pchild)
{
   mChildren.remove(pchild);
}

void ASTNode::detachChild(const ASTNode& child)
{
   mChildren.detach(child);
}

ASTNodes& ASTNode::getChildren()
{
   return mChildren;
}

