
#ifndef AST_NODES_H_
#define AST_NODES_H_

#include <vector>

class ASTNode;
class ASTVisitor;
class CompileContext;

class ASTNodes
{
   typedef std::vector<ASTNode*> Nodes;

public:
   ASTNodes();
   ~ASTNodes();

   void add(ASTNode* pnode);
   void remove(ASTNode** pnode);
   void detach(const ASTNode& node);

 // query
   bool isEmpty() const;
   int size() const;

   int indexOf(const ASTNode& node) const;

   const ASTNode& operator[](int index) const;
         ASTNode& operator[](int index);

 // visitor
   void accept(ASTVisitor& visitor) const;
   void accept(ASTVisitor& visitor);

private:
   Nodes mNodes;
};

#endif // AST_NODES_H_
