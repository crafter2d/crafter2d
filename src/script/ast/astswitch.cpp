
#include "astswitch.h"

#include <sstream>

#include "core/defines.h"

#include "script/compiler/compilecontext.h"

#include "astvisitor.h"
#include "astexpression.h"
#include "astcase.h"

ASTSwitch::ASTSwitch():
   ASTStatement(),
   mpExpression(NULL)
{
}

ASTSwitch::~ASTSwitch()
{
   setExpression(NULL);
}

// - Get/set

const ASTNode& ASTSwitch::getExpression() const
{
   ASSERT_PTR(mpExpression);
   return *mpExpression;
}

ASTNode& ASTSwitch::getExpression()
{
   ASSERT_PTR(mpExpression);
   return *mpExpression;
}

void ASTSwitch::setExpression(ASTNode* pexpression)
{
   delete mpExpression;
   mpExpression = pexpression;
}

// - Query
   
int ASTSwitch::getDefaultCount() const
{
   int count = 0;
   const ASTNodes& cases = getChildren();
   for ( int index = 0; index < cases.size(); index++ )
   {
      const ASTCase& ast = dynamic_cast<const ASTCase&>(cases[index]);
      if ( ast.isDefault() )
      {
         count++;
      }
   }
   return count;
}

// - Operations
   
void ASTSwitch::validateCaseTypes(CompileContext& context, ASTType& expectedtype)
{
   ASSERT(expectedtype.isInt() || expectedtype.isReal());

   const ASTNodes& cases = getChildren();
   for ( int index = 0; index < cases.size(); index++ )
   {
      const ASTCase& ast = dynamic_cast<const ASTCase&>(cases[index]);
      if ( ast.isCase() && !expectedtype.greater(ast.getType()) )
      {
         std::stringstream ss;
         ss << "Case " << index << " should be of type " << expectedtype.toString();
         context.getLog().error(ss.str());
      }
   }
}

// - Visitor

ACCEPT_IMPL(ASTSwitch);
