
#include "compilestep.h"

#include "core/defines.h"

#include "ast/astclass.h"
#include "compilecontext.h"

CompileStep::CompileStep(CompileContext& context):
   mContext(context),
   mpClass(NULL)
{
}

// - Operations
   
bool CompileStep::step(ASTNode& node)
{
   return performStep(node);
}

// - Error reporting

void CompileStep::warning(int errnr, const String& msg, const ASTNode& node)
{
   warning(errnr, msg, node.getPosition());
}

void CompileStep::warning(int errnr, const String& msg, const AntlrTextPosition& pos)
{
   ASSERT_PTR(mpClass);
   mContext.getLog().warning(errnr, mpClass->getFullName(), msg, pos);
}

void CompileStep::error(int errnr, const String& msg, const ASTNode& node)
{
   error(errnr, msg, node.getPosition());
}

void CompileStep::error(int errnr, const String& msg, const AntlrTextPosition& pos)
{
   ASSERT_PTR(mpClass);
   mContext.getLog().error(errnr, mpClass->getFullName(), msg, pos);
}
