
#include "astswitch.h"

#include <sstream>

#include "core/conv/numberconverter.h"
#include "core/defines.h"

#include "yasc/compiler/compilecontext.h"

#include "astvisitor.h"
#include "astexpression.h"
#include "astcase.h"

ASTSwitch::ASTSwitch():
   ASTStatement(),
   mpExpression(NULL),
   mpType(NULL)
{
}

ASTSwitch::~ASTSwitch()
{
   setExpression(NULL);
   setType(NULL);
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

const ASTCase& ASTSwitch::getCase(int index) const
{
   ASSERT(index < getChildren().size());
   return dynamic_cast<const ASTCase&>(getChildren()[index]);
}

const ASTType& ASTSwitch::getType() const
{
   ASSERT_PTR(mpType);
   return *mpType;
}

void ASTSwitch::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
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

int ASTSwitch::getTotalCount() const
{
   return getChildren().size();
}

bool ASTSwitch::hasDefault() const
{
   return getDefaultCount() != 0;
}

bool ASTSwitch::canLookup() const
{
   const ASTNodes& cases = getChildren();
   for ( int index = 0; index < cases.size(); index++ )
   {
      const ASTCase& ast = dynamic_cast<const ASTCase&>(cases[index]);
      if ( !ast.isDefault() && !ast.hasValue() )
      {
         return false;
      }
   }
   return true;
}

bool ASTSwitch::hasReturn(bool& hasunreachablecode) const
{
   bool result = true;
   const ASTNodes& cases = getChildren();
   for ( int index = 0; index < cases.size(); index++ )
   {
      const ASTCase& ast = dynamic_cast<const ASTCase&>(cases[index]);
      const ASTStatement& statement = static_cast<const ASTStatement&>(ast.getBody());
      
      if ( !statement.hasReturn(hasunreachablecode) )
      {
         result = false;
      }
   }

   return result;
}

// - Operations
   
void ASTSwitch::validateCaseTypes(CompileContext& context)
{
   ASSERT_PTR(mpType);
   ASSERT(mpType->isValueType());

   const ASTNodes& cases = getChildren();
   for ( int index = 0; index < cases.size(); index++ )
   {
      const ASTCase& ast = dynamic_cast<const ASTCase&>(cases[index]);
      if ( ast.isCase() && !mpType->greater(ast.getType()) )
      {
         String number;
         String ss;
         ss = String("Case ") + NumberConverter::getInstance().format(number, index) + String(" should be of type ") + mpType->toString();
         context.getLog().error(ss);
      }
   }
}

// - Visitor

ACCEPT_IMPL(ASTSwitch);