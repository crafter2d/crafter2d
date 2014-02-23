
#include "prettyprintvisitor.h"

#include <iostream>

#include "yasc/compiler/ast/ast.h"

using std::cout;
using std::cerr;
using std::endl;

PrettyPrintVisitor::PrettyPrintVisitor():
   ASTVisitor()
{
}

void PrettyPrintVisitor::visit(const ASTRoot& root)
{
   visitChildren(root);
}

void PrettyPrintVisitor::visit(const ASTCompound& compound)
{
   cout << " ( ";
   visitChildren(compound);
   cout << " ) ";
}

void PrettyPrintVisitor::visit(const ASTConcatenate& concatenate)
{
   concatenate.getLeft().accept(*this);

   if ( concatenate.hasRight() )
   {
      switch ( concatenate.getMode() )
      {
         case ASTConcatenate::eAnd: cout << " and "; break;
         case ASTConcatenate::eOr:  cout << " or ";  break;
      }

      concatenate.getRight().accept(*this);
   }
}
