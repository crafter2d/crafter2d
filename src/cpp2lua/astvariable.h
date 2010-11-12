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
#ifndef AST_VARIABLE_H_
#define AST_VARIABLE_H_

#include <string>

#include "astdeclaration.h"
#include "asttype.h"
#include "astexpression.h"

class ASTVariable : public ASTDeclaration
{
public:
   ASTVariable(ASTType* ptype, std::string* pname, ASTExpression* passignment = NULL):
      ASTDeclaration(),
      mpType(ptype),
      mName(*pname), 
      mpAssignment(passignment)
   {
   }

   virtual ~ASTVariable() 
   {
      delete mpType;
   }

   const std::string& getName() const { return mName; }
   ASTType& getType() { return *mpType; }

protected:
   virtual void doPrettyPrint()
   {
      mpType->prettyPrint();
      cout << " " << mName;

      if ( mpAssignment != 0 )
      {
         cout << "=";
         mpAssignment->prettyPrint();
      }
   }

   virtual bool doValidate()
   {
      return mpType != NULL && mName.length() > 0;
   }

private:
   ASTType*       mpType;
   ASTExpression* mpAssignment;
   std::string    mName;
};

#endif // AST_VARIABLE_H_

