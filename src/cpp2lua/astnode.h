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
#ifndef AST_NODE_H_
#define AST_NODE_H_

#include <vector>
#include <iostream>

using std::cout;
using std::endl;

class CodeStream;

class ASTNode
{
public:
   typedef std::vector<ASTNode*> Children;

   enum CodePhase
   {
      eFirstPhase,
      eSecondPhase
   };

   ASTNode():
      mChildren(),
      mpParent(0)
   {
   }

   virtual ~ASTNode()
   {
      for ( int index = 0; index < mChildren.size(); index++ )
      {
         ASTNode* pchild = mChildren[index];
         delete pchild;
      }
      mChildren.clear();

      mpParent = 0;
   }

   void add(ASTNode* pchild)
   {
      mChildren.push_back(pchild);
      pchild->mpParent = this;
   }

   Children& getChildren() { return mChildren; }
   ASTNode&  getParent() { return *mpParent; }

   void prettyPrint() 
   {
      doPrettyPrint();
      for ( int index = 0; index < mChildren.size(); index++ )
      {
         ASTNode* pchild = mChildren[index];
         pchild->prettyPrint();

         doPrettyBetween(index+1 == mChildren.size());
      }
      doPrettyEnd();
   }

   bool validate()
   {
      if ( !doValidate() )
         return false;

      for ( int index = 0; index < mChildren.size(); index++ )
      {
         ASTNode* pchild = mChildren[index];
         if ( !pchild->validate() )
            return false;
      }
      return true;
   }

   void generateCode(CodeStream& stream, CodePhase phase)
   {
      doGenerateCodeBegin(stream, phase);

      for ( int index = 0; index < mChildren.size(); index++ )
      {
         ASTNode* pchild = mChildren[index];
         pchild->generateCode(stream, phase);
      }

      doGenerateCodeEnd(stream, phase);
   }

protected:

   virtual void doPrettyPrint() {}
   virtual void doPrettyBetween(bool last) {}
   virtual void doPrettyEnd() {}

   virtual bool doValidate() { return true; }

   virtual void doGenerateCodeBegin(CodeStream& stream, CodePhase phase) {}
   virtual void doGenerateCodeEnd(CodeStream& stream, CodePhase phase) {}

private:
   Children mChildren;
   ASTNode* mpParent;
};

#endif // AST_NODE_H_
