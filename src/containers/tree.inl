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
#include "../defines.h"

template<class E>
INLINE Tree<E>::Tree():
   _proot(NULL)
{
}

template<class E>
INLINE Tree<E>::~Tree()
{
}

template<class E>
bool Tree<E>::hasRoot() const
{
   return _proot != NULL;
}

template<class E>
INLINE const TreeNode<E>& Tree<E>::getRoot() const
{
   ASSERT_PTR(_proot)
   return *_proot;
}

template<class E>
INLINE TreeNode<E>& Tree<E>::getRoot()
{
   ASSERT_PTR(_proot)
   return *_proot;
}

template<class E>
INLINE void Tree<E>::insert(TreeNode<E>* pposition, E& element)
{
   TreeNode<E>* pnode = new TreeNode<E>(&element, pposition);

   if ( pposition == NULL )
      _proot = pnode;
}

template<class E>
INLINE void Tree<E>::insert(Iterator<E>& it, E& element)
{
}

template<class E>
INLINE TreeNode<E>* Tree<E>::findElement(const E& element)
{
   return _proot != NULL ? findElement(*_proot, element) : NULL;
}

template<class E>
INLINE TreeNode<E>* Tree<E>::findElement(TreeNode<E>& node, const E& element)
{
   if ( &node.getData() == &element )
      return &node;
   else if ( node.hasChildren() )
   {
      ListIterator< TreeNode<E> > it(node.getChildren());
      while ( it.isValid() )
      {
         return findElement(it.item(), element);
         ++it;
      }
   }

   return NULL;
}
