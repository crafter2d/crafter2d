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
#ifndef PRELOAD_VISITOR_H_
#define PRELOAD_VISITOR_H_

#include "core/string/string.h"

#include "script/compiler/compilestep.h"
#include "script/compiler/classresolver.h"
#include "script/scope/scopestack.h"

class ASTVariable;
class ASTType;
class CompileContext;

class PreloadVisitor : public CompileStep
{
public:
   PreloadVisitor(CompileContext& context);
   virtual ~PreloadVisitor();

 // visiting
   virtual void visit(ASTRoot& ast);
   virtual void visit(ASTPackage& ast);
   virtual void visit(ASTUse& ast);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTFunctionArgument& ast);
   virtual void visit(ASTField& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTLocalVariable& ast);
   virtual void visit(ASTExpressionStatement& ast);
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
   virtual void visit(ASTCast& ast);
   virtual void visit(ASTAccess& ast);
   virtual void visit(ASTCompound& compound);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
 // class loading
   bool load(ASTType& type);
   bool tryLoad(ASTType& type);

 // operations
   void checkVarInit(ASTVariable& var, bool allowarray);
   void checkStaticAccess(ASTUnary& unary);

   CompileContext&   mContext;
   ClassResolver     mClassResolver;
   String            mPackage;
   ScopeStack        mScopeStack;
   ASTClass*         mpClass;
};

#endif // PRELOAD_VISITOR_H_
