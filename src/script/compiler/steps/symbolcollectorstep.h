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
#ifndef SYMBOL_COLLECTOR_VISITOR_H_
#define SYMBOL_COLLECTOR_VISITOR_H_

#include "../compilestep.h"
#include "../classresolver.h"

#include <vector>

class ASTClass;
class ASTVariable;
class ASTType;
class CompileContext;

/// \brief Resolves all types in an AST tree. Types which can not be resolved 
/// are marked as unknown. This step will not log any failures.

class SymbolCollectorVisitor : public CompileStep
{
public:
   explicit SymbolCollectorVisitor(CompileContext& context);
   virtual ~SymbolCollectorVisitor();
   
 // visitor
   virtual void visit(ASTRoot& ast);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTFunctionArgument& ast);
   virtual void visit(ASTField& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTVariableInit& ast);
   virtual void visit(ASTArrayInit& ast);
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
 // operations
   void createDefaultConstructor(ASTClass& ast);
   void resolveType(ASTType& type);

   CompileContext&  mContext;
   ClassResolver    mResolver;
   ASTClass*        mpClass;
};

#endif // SYMBOL_COLLECTOR_VISITOR_H_
