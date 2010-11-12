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
#ifndef AST_CLASS_H_
#define AST_CLASS_H_

#include <string>

#include "astnode.h"

#include "codestream.h"
#include "codeblock.h"

class ASTClass : public ASTNode
{
public:
   ASTClass(std::string* pname): mName(*pname), mBase() {}
   ASTClass(std::string* pname, std::string* pbase): mName(*pname), mBase(*pbase) {}


   const std::string& getName() const { return mName; }

protected:
   virtual void doPrettyPrint() {
      cout << "class " << mName;
      if ( mBase.length() > 0 )
         cout << " : public " << mBase;
      cout << endl;
   }

   virtual void doPrettyEnd() {
      cout << ";" << endl;
   }

   virtual void doGenerateCodeBegin(CodeStream& stream, CodePhase phase)
   {
      if ( phase == eSecondPhase )
      {
         stream << "{" << CodeBlockedStream::endline;
         CodeBlockedStream block(stream);
         block << "ScriptClass theclass(scriptlib, \"" << mName << "\", \"" << mBase << "\");" << CodeBlockedStream::endline;
      }
   }

   virtual void doGenerateCodeEnd(CodeStream& stream, CodePhase phase)
   {
      if ( phase == eSecondPhase )
      {
         stream << "}" << CodeBlockedStream::endline;
      }
   }

private:
   std::string mName;
   std::string mBase;
};

#endif // AST_CLASS_H_
