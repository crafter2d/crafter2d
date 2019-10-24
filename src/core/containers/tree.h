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

#include "iterator.h"
#include "treenode.h"
#include "treedepthfirstiterator.h"

template <class E>
class Tree
{
public:
   using iterator = TreeDepthFirstIterator<E>;

   Tree() : _root() {}
   ~Tree() {}

 // get/set interface
   const TreeNode<E>& getRoot() const { return _root; }
   TreeNode<E>& getRoot() { return _root; }

 // iterators
   iterator begin() {
      return TreeDepthFirstIterator<E>(_root);
   }

   iterator end() {
      return TreeDepthFirstIterator<E>(_root);
   }

 // query
   size_t count() const {
      return _root.count();
   }

 // operations
   TreeNode<E>& insert_child(iterator& pos, E& element) {
      TreeNode<E>& node = static_cast<TreeIterator<E>&>(pos).getNode();
      return node.insert_child(element);
   }

   TreeNode<E>* find(const E& element) {
      return _root.find(element);
   }

private:

 // data
   TreeNode<E> _root;
};

#endif
