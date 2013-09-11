/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SYMBOL_CHECK_VISITOR_H_
#define SYMBOL_CHECK_VISITOR_H_

#include "script/compiler/compilestep.h"

#include "script/ast/asttype.h"
#include "script/ast/astunary.h"
#include "script/scope/scopestack.h"

class CompileContext;
class ASTAccess;
class ASTClass;
class ASTClassTable;
class ASTLiteralTable;
class ASTFunctionBase;
class ASTVariable;

class SymbolCheckVisitor : public CompileStep
{
public:
   SymbolCheckVisitor(CompileContext& context);
   
 // visiting
   virtual void visit(ASTRoot& root);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTFunctionArgument& ast);
   virtual void visit(ASTField& ast);
   virtual void visit(ASTProperty& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTVariableInit& ast);
   virtual void visit(ASTArrayInit& ast);
   virtual void visit(ASTExpressionStatement& ast);
   virtual void visit(ASTLocalVariable& ast);
   virtual void visit(ASTIf& ast);
   virtual void visit(ASTFor& ast);
   virtual void visit(ASTForeach& ast);
   virtual void visit(ASTWhile& ast);
   virtual void visit(ASTDo& ast);
   virtual void visit(ASTSwitch& ast);
   virtual void visit(ASTCase& ast);
   virtual void visit(ASTReturn& ast);
   virtual void visit(ASTTry& ast);
   virtual void visit(ASTCatch& ast);
   virtual void visit(ASTThrow& ast);
   virtual void visit(ASTAssert& ast);
   virtual void visit(ASTExpression& ast);
   virtual void visit(ASTConcatenate& ast);
   virtual void visit(ASTUnary& ast);
   virtual void visit(ASTInstanceOf& ast);
   virtual void visit(ASTNew& ast);
   virtual void visit(ASTSuper& ast);
   virtual void visit(ASTNative& ast);
   virtual void visit(ASTLiteral& ast);
   virtual void visit(ASTCast& ast);
   virtual void visit(ASTAccess& ast);
   virtual void visit(ASTCompound& compound);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
 // query
   bool isVariable(const ASTNode& node) const;

 // operations
   void checkVarInit(ASTVariable& var);
   void checkReturn(const ASTFunction& function);
   void checkFunctionAccess(const ASTClass& aclass, ASTAccess& access, bool isstatic);
   void checkOperator(ASTUnary::Operator op);
   void checkUnknown(const ASTType& type);

   CompileContext& mContext;
   ASTClass*       mpClass;
   ASTFunction*    mpFunction;
   ASTType         mCurrentType;
   ScopeStack      mScopeStack;
   bool            mStatic;
};
#endif // SYMBOL_CHECK_VISITOR_H_
