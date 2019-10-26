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
#ifndef TREE_H
#define TREE_H

#include <type_traits>

#include "treeiterator.h"
#include "treenode.h"
#include "treetraversal.h"

template <class E>
class Tree
{
public:
   using iterator = TreeIterator<E>;

   Tree() : _root() {}
   ~Tree() {}

 // get/set interface
   const TreeNode<E>& getRoot() const { return _root; }
   TreeNode<E>& getRoot() { return _root; }

 // iterators
   iterator begin() {
      return TreeIterator<E>(*this, new TreeDepthFirstTraversal<E>(_root));
   }

   iterator end() {
      return TreeIterator<E>(*this, nullptr);
   }

 // query
   size_t count() const {
      return _root.count();
   }

 // operations
   TreeNode<E>& insert_child(iterator& pos, const E& element) {
      TreeNode<E>* pnode = static_cast<TreeIterator<E>&>(pos).node();
      if ( pnode )
         return pnode->insert_child(element);
      throw std::invalid_argument("Invalid iterator");
   }

   TreeNode<E>& insert_child(iterator& pos, E&& element) {
      TreeNode<E>* pnode = static_cast<TreeIterator<E>&>(pos).node();
      if ( pnode )
         return pnode->insert_child(std::move(element));
      throw std::invalid_argument("Invalid iterator");
   }

   TreeNode<E>* find(const E& element) {
      return _root.find(element);
   }

private:

 // data
   TreeNode<E> _root;
};

#endif
