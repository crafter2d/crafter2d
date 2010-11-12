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
      mNamespace(),
      mCustom(*pcustom),
      mConst(false)
   {
      if ( pnamespace != 0 )
         mNamespace = *pnamespace;
   }

   Type getType() { return mType; }
   std::string getCustomType() { 
      std::string result;
      if ( mNamespace.length() > 0 )
         result = mNamespace + "::";
      result += mCustom;
      return result;
   }

   void isConst(bool isconst) { mConst = isconst; }
   void setReference() { mPassBy = eReference; }
   void setPointer() { mPassBy = ePointer; }
   bool isPointer() const { return mPassBy == ePointer; }

   std::string asString() const
   {
      std::string result;
      if ( mConst )
         result = "const ";

      switch ( mType )
      {
         case eVoid:       result += "void";         break;
         case eInt:        result += "int";          break;
         case eFloat:      result += "float";        break;
         case eBool:       result += "bool";         break;
         case eChar:       result += "char";         break;
         case eCustom:
            {
               if ( mNamespace.length() > 0 )
                  result += mNamespace + "::";
               result += mCustom;
            }
            break;
         default:
            break;
      }
      switch ( mPassBy )
      {
         case eReference:  result += "&";   break;
         case ePointer:    result += "*";   break;
         case eValue:                       break;
      }
      return result;
   }

protected:
   virtual void doPrettyPrint() {
      std::string representation = asString();
      cout << representation;

      
   }
   
private:
   Type        mType;
   PassBy      mPassBy;
   std::string mNamespace;
   std::string mCustom;
   bool        mConst;
};

#endif // AST_TYPE_H_
