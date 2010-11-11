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
#ifndef AST_TYPE_H_
#define AST_TYPE_H_

#include <string>

#include "astnode.h"

class ASTType : public ASTNode
{
public:
   enum Type {
      eVoid,
      eInt,
      eFloat,
      eBool,
      eChar,
      eCustom
   };

   enum PassBy {
      eReference,
      ePointer,
      eValue
   };

   ASTType(Type type): 
      mType(type),
      mPassBy(eValue),
      mNamespace(),
      mCustom(),
      mConst(false)
   {
   }
  
   ASTType(std::string* pcustom, std::string* pnamespace = 0):
      mType(eCustom),
      mPassBy(eValue),
      mNamespace(*pnamespace),
      mCustom(*pcustom),
      mConst(false)
   {
   }

   void isConst(bool isconst) { mConst = isconst; }
   void isReference() { mPassBy = eReference; }
   void isPointer() { mPassBy = ePointer; }

protected:
   virtual void doPrettyPrint() {
      if ( mConst )
         cout << "const ";

      switch ( mType )
      {
         case eVoid:       cout << "void";         break;
         case eInt:        cout << "int";          break;
         case eFloat:      cout << "float";        break;
         case eBool:       cout << "bool";         break;
         case eChar:       cout << "char";         break;
         case eCustom:
            {
               if ( mNamespace.length() > 0 )
                  cout << mNamespace << "::";
               cout << mCustom;
            }
            break;
         default:          cout << "<unknown>";    break;
      };

      switch ( mPassBy )
      {
         case eReference:  cout << "&";   break;
         case ePointer:    cout << "*";   break;
         case eValue:                     break;
      }
   }

private:
   Type        mType;
   PassBy      mPassBy;
   std::string mNamespace;
   std::string mCustom;
   bool        mConst;
};

#endif // AST_TYPE_H_
