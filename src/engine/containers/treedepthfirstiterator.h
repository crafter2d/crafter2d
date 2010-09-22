/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#ifndef TREE_DEPTH_FIRST_ITERATOR_H_
#define TREE_DEPTH_FIRST_ITERATOR_H_

#include <stack>
#include "treeiterator.h"

template<class E>
class TreeDepthFirstIterator : public TreeIterator<E>
{
public:
   typedef TreeNode<E>* TreeHandle;

   TreeDepthFirstIterator(Tree<E>& tree);

 // overloads
   virtual void     operator++();

   virtual void* key();
   virtual void  reset();
   virtual E&    item();

   virtual bool  isValid() const;

 // operations
   void skipBranch();

private:
   void push(TreeHandle pnode);

   class StackNode
   {
   public:
      TreeHandle pnode;
      int child;
   };

   std::stack<StackNode> _stack;
};

#include "treedepthfirstiterator.inl"

#endif
