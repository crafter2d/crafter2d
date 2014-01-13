
#include "astnode.h"

ASTNode::ASTNode():
   mChildren(),
   mLine(-1),
   mPos(-1)
{
}

ASTNode::~ASTNode()
{
}

// - Get/set

int ASTNode::getLine() const
{
   return mLine;
}

void ASTNode::setLine(int line)
{
   mLine = line;
}

int ASTNode::getPos() const
{
   return mPos;
}

void ASTNode::setPos(int pos)
{
   mPos = pos;
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

