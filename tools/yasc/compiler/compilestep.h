
#ifndef COMPILE_STEP_H_
#define COMPILE_STEP_H_

#include "ast/astvisitor.h"

class ASTClass;
class ASTNode;
class AntlrTextPosition;
class CompileContext;
class String;

class CompileStep : public ASTVisitor
{
public:
   CompileStep(CompileContext& context);

 // operations
   bool step(ASTNode& node);

protected:
 // interface
   virtual bool performStep(ASTNode& node) = 0;

 // error reporting
   void warning(int errnr, const String& msg, const ASTNode& node);
   void warning(int errnr, const String& msg, const AntlrTextPosition& pos);

   void error(int errnr, const String& msg, const ASTNode& node);
   void error(int errnr, const String& msg, const AntlrTextPosition& pos);

 // data
   CompileContext& mContext;
   ASTClass*       mpClass;
};

#endif // COMPILE_STEP_H_
