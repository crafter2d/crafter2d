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
#ifndef ANTLR_INTERFACE_H_
#define ANTLR_INTERFACE_H_

#include <antlr3interfaces.h>

#include "script/ast/ast.h"
#include "script/compiler/classresolver.h"

class QString;
class QStringList;

class ASTClass;
class AntlrNode;
class AntlrStream;
class CompileContext;

class AntlrParser
{
public:
   AntlrParser(CompileContext& context);

 // parse
   ASTRoot* parse(const AntlrStream& stream);

private:
 // helpers
   ASTNode* handleTree(const AntlrNode& node);
   void     handleFuncArguments(const AntlrNode& node, ASTFunction& function);
   void     handleModifiers(const AntlrNode& node, ASTModifiers& modifiers);

   ASTType* getType(const AntlrNode& node);

 // specific
   ASTNode*                handlePackage(const AntlrNode& node);
   ASTNode*                handleUse(const AntlrNode& node);
   ASTTypeVariables*       handleTypeVariables(const AntlrNode& node);

   ASTNode*                handleClass(const AntlrNode& node);
   ASTMember*              handleConstructor(const AntlrNode& node);
   ASTMember*              handleFuncDecl(const AntlrNode& node);
   ASTMember*              handleVoidFuncDecl(const AntlrNode& node);
   ASTMember*              handleVarDecl(const AntlrNode& node);
   ASTMember*              handleInterfaceMember(const AntlrNode& node);
   ASTMember*              handleInterfaceVoidMember(const AntlrNode& node);
   ASTAnnotations*         handleAnnotations(const AntlrNode& node);
   ASTBlock*               handleBlock(const AntlrNode& node);
   ASTLocalVariable*       handleLocalVarDecl(const AntlrNode& node);
   ASTIf*                  handleIf(const AntlrNode& node);
   ASTFor*                 handleFor(const AntlrNode& node);
   ASTForeach*             handleForeach(const AntlrNode& node);
   ASTWhile*               handleWhile(const AntlrNode& node);
   ASTDo*                  handleDo(const AntlrNode& node);
   ASTSwitch*              handleSwitch(const AntlrNode& node);
   ASTCase*                handleCase(const AntlrNode& node);
   ASTReturn*              handleReturn(const AntlrNode& node);
   ASTTry*                 handleTry(const AntlrNode& node);
   ASTCatch*               handleCatch(const AntlrNode& node);
   ASTThrow*               handleThrow(const AntlrNode& node);
   ASTAssert*              handleAssert(const AntlrNode& node);
   ASTLoopControl*         handleLoopControl(const AntlrNode& node);
   ASTExpressionStatement* handleExpressionStatement(const AntlrNode& node);
   ASTCompound*            handleCompound(const AntlrNode& node);
   ASTExpression*          handleExpression(const AntlrNode& node);
   ASTNode*                handleInstanceOf(const AntlrNode& node);
   ASTUnary*               handleUnary(const AntlrNode& node);
   ASTNew*                 handleNew(const AntlrNode& node);
   ASTNode*                handleSuper(const AntlrNode& node);
   ASTCast*                handleCast(const AntlrNode& node);
   ASTAccess*              handleAccess(const AntlrNode& node);
   ASTAccess*              handleArrayAccess(const AntlrNode& node);
   ASTAccess*              handleClassAccess(const AntlrNode& node);
   ASTNode*                handleConcatenate(const AntlrNode& node);
   ASTLiteral*             handleLiteral(const AntlrNode& node);

   CompileContext& mContext;
   ClassResolver   mClassResolver;
   std::string     mPackage;
   ASTClass*       mpClass;
};

#endif // ANTLR_INTERFACE_H_
