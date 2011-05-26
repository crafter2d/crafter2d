
#include "astnodes.h"

#include <algorithm>

#include "astnode.h"
#include "astvisitor.h"

ASTNodes::ASTNodes()
{
}

ASTNodes::~ASTNodes()
{
}

const ASTNode& ASTNodes::operator[](int index) const
{
   return *mNodes[index];
}

ASTNode& ASTNodes::operator[](int index)
{
   return *mNodes[index];
}

void ASTNodes::add(ASTNode* pnode)
{
   mNodes.push_back(pnode);
}

void ASTNodes::set(int index, ASTNode* pnode)
{
   mNodes[index] = pnode;
}

void ASTNodes::remove(ASTNode** pnode)
{
   int index = indexOf(**pnode);
   mNodes.erase(mNodes.begin() + index);

   delete *pnode;
   *pnode = NULL;
}

void ASTNodes::detach(const ASTNode& node)
{
   int index = indexOf(node);
   mNodes.erase(mNodes.begin() + index);
}

// - Query

bool ASTNodes::isEmpty() const
{
   return mNodes.empty();
}

int ASTNodes::size() const
{
   return mNodes.size();
}

int ASTNodes::indexOf(const ASTNode& node) const
{
   Nodes::const_iterator it = std::find(mNodes.begin(), mNodes.end(), &node);
   return it - mNodes.begin();
}

// - Visitor

void ASTNodes::accept(ASTVisitor& visitor) const
{
   for ( std::size_t index = 0; index < mNodes.size(); index++ )
   {
      const ASTNode* pnode = mNodes[index];
      pnode->accept(visitor);
   }
}
   
void ASTNodes::accept(ASTVisitor& visitor)
{
   for ( std::size_t index = 0; index < mNodes.size(); index++ )
   {
      ASTNode* pnode = mNodes[index];
      pnode->accept(visitor);
   }
}