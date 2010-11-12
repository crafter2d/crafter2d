/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef AST_FUNCTION_H_
#define AST_FUNCTION_H_

#include <string>

#include "astdeclaration.h"
#include "asttype.h"
#include "astvariables.h"

class ASTFunction : public ASTDeclaration
{
public:
   enum Type {
      eConstructor,
      eDestructor,
      eMember
   };

   ASTFunction(Type type, std::string* pname, ASTVariables* parguments = 0):
      ASTDeclaration(),
      mType(type),
      mpType(0),
      mpArguments(parguments),
      mName(*pname),
      mFunction(),
      mConst(false)
   {
   }

   ASTFunction(ASTType* ptype, std::string* pname, ASTVariables* parguments = 0, bool isconst = false):
      ASTDeclaration(), 
      mType(eMember), 
      mpType(ptype),
      mpArguments(parguments),
      mName(*pname), 
      mFunction(),
      mConst(isconst)
   {
   }

   virtual ~ASTFunction() 
   {
      delete mpType;
      delete mpArguments;
   }

protected:
   virtual void doPrettyPrint()
   {
      switch ( mType )
      {
         case eConstructor:
            cout << mName << "(";
            break;
         case eDestructor:
            cout << "~" << mName << "()";
            return;
         case eMember:
            mpType->prettyPrint();
            cout << " " << mName << "(";
            break;
         default:
            cout << "<unknown function>";
            break;
      }

      if ( mpArguments != 0 )
         mpArguments->prettyPrint();

      cout << ")";
      if ( mConst )
         cout << " const";
   }
   
   virtual bool doValidate();

   virtual void doGenerateCodeBegin(CodeStream& stream, CodePhase phase);

private:
   void generateDeclaration(CodeStream& stream);
   void generateImplementation(CodeStream& stream);

   Type           mType;
   ASTType*       mpType;
   ASTVariables*  mpArguments;
   std::string    mName;
   std::string    mFunction;
   bool           mConst;
};

#endif // AST_FUNCTION_H_
