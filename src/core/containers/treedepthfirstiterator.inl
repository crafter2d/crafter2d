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
TreeDepthFirstIterator<E>::TreeDepthFirstIterator(TreeNode<E>& node):
   TreeIterator<E>(node),
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
      return _stack.back().pnode;
}

template<class E>
void TreeDepthFirstIterator<E>::reset()
{
   auto node = std::move(_stack.front());
   _stack.clear();
   _stack.push_back(StackNode(node));
}

template<class E>
void TreeDepthFirstIterator<E>::operator++()
{
   StackNode& node = _stack.back();

   if ( node.child < node.pnode->getChildren().size() && node.pnode->isExpanded() )
   {
      push(node.pnode->getChild(node.child));

      node.child++;
   }
   else
   {
      while ( !_stack.empty() )
      {
         _stack.pop_back();

         if ( !_stack.empty() && _stack.back().child < _stack.back().pnode->getChildren().size() )
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
   StackNode& node = _stack.back();
   return node.pnode->getData();
}

template<class E>
bool TreeDepthFirstIterator<E>::isValid() const
{
   if ( _stack.empty() || ( _stack.size() == 1 && _stack.back().child >= _stack.back().pnode->getChildren().size() ) )
      return false;
   else
      return true;
}

template<class E>
INLINE void TreeDepthFirstIterator<E>::push(TreeHandle pnode)
{
   _stack.emplace_back(pnode);
}

template <class E>
INLINE void TreeDepthFirstIterator<E>::skipBranch()
{
   _stack.top().child++;
}
