
#ifndef AST_TRY_H_
#define AST_TRY_H_

#include "aststatement.h"

class ASTBlock;
class ASTCatch;

class ASTTry : public ASTStatement
{
public:
   ASTTry();
   virtual ~ASTTry();

 // get/set
   const ASTBlock& getBody() const;
         ASTBlock& getBody();
   void            setBody(ASTBlock* pbody);

   bool            hasFinallyBlock() const;
   const ASTBlock& getFinallyBlock() const;
         ASTBlock& getFinallyBlock();
   void            setFinallyBlock(ASTBlock* pblock);

   int  getResourceIndex() const;
   void setResourceIndex(int index);

   const ASTNodes& getCatches() const;

 // query
   bool hasReturn(bool& hasunreachablecode) const;

 // operations
   void addCatch(ASTCatch* pcatch);

 // visitor
   ACCEPT;

private:
   ASTBlock* mpBody;
   ASTBlock* mpFinally;
   int       mResourceIndex;
};

#endif // AST_TRY_H_
