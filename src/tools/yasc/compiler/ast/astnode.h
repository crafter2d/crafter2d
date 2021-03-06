
#ifndef AST_NODE_H_
#define AST_NODE_H_

#include "core/defines.h"

#include "../antlr/antlrtextposition.h"
#include "astnodes.h"

class ASTVisitor;
class CompileContext;

class ASTNode
{
public:
   ASTNode();
   virtual ~ASTNode();

 // get/set
   const AntlrTextPosition& getPosition() const;
   void                     setPosition(const AntlrTextPosition& pos);

 // children
   bool hasChildren() const;
   void addChild(ASTNode* pchild);
   void insertChild(int index, ASTNode* pchild);
   void removeChild(ASTNode** pchild);
   void detachChild(const ASTNode& child);

 // visiting
   virtual void accept(ASTVisitor& visitor) const = 0;
   virtual void accept(ASTVisitor& visitor) = 0;

protected:
 // get/set
   const ASTNodes& getChildren() const;
         ASTNodes& getChildren();

private:
   friend class ASTVisitor;

   ASTNodes          mChildren;
   AntlrTextPosition mPosition;
};

#define ACCEPT \
   virtual void accept(ASTVisitor& visitor) const; \
   virtual void accept(ASTVisitor& visitor)

#define ACCEPT_IMPL(aclass) \
   void aclass::accept(ASTVisitor& visitor) const { visitor.visit(*this); } \
   void aclass::accept(ASTVisitor& visitor) { visitor.visit(*this); }

#endif // AST_NODE_H_
