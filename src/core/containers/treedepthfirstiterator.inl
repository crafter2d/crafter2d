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
#include "core/defines.h"

template<class E>
TreeDepthFirstIterator<E>::TreeDepthFirstIterator(Tree<E>& tree):
   TreeIterator<E>(tree),
   _stack()
{
   reset();
}

template<class E>
void* TreeDepthFirstIterator<E>::key()
{
   if ( _stack.empty() )
      return nullptr;
   else
      return _stack.top().pnode;
}

template<class E>
void TreeDepthFirstIterator<E>::reset()
{
   Tree<E> tree = TreeIterator<E>::_tree;
   if ( tree.hasRoot() )
   {
      push(&tree.getRoot());
   }
}

template<class E>
void TreeDepthFirstIterator<E>::operator++()
{
   StackNode& node = _stack.top();

   if ( node.child < node.pnode->getChildren().size() && node.pnode->isExpanded() )
   {
      push(node.pnode->getChild(node.child));

      node.child++;
   }
   else
   {
      while ( !_stack.empty() )
      {
         _stack.pop();

         if ( !_stack.empty() && _stack.top().child < _stack.top().pnode->getChildren().size() )
         {
            operator++();
            return;
         }
      }
   }
}

template<class E>
E& TreeDepthFirstIterator<E>::item()
{
   ASSERT(!_stack.empty());
   StackNode& node = _stack.top();
   return node.pnode->getData();
}

template<class E>
bool TreeDepthFirstIterator<E>::isValid() const
{
   if ( _stack.empty() || ( _stack.size() == 1 && _stack.top().child >= _stack.top().pnode->getChildren().size() ) )
      return false;
   else
      return true;
}

template<class E>
INLINE void TreeDepthFirstIterator<E>::push(TreeHandle pnode)
{
   StackNode newnode;
   newnode.pnode = pnode;
   newnode.child = 0;

   _stack.push(newnode);
}

template <class E>
INLINE void TreeDepthFirstIterator<E>::skipBranch()
{
   _stack.top().child++;
}
