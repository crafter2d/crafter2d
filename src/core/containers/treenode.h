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
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <vector>

template <class E>
class TreeNode
{
public:
 // types
   using Children = std::vector<TreeNode<E>>;

 // constructors
            TreeNode(): _children(), _data() {}
   explicit TreeNode(const E& data): _children(), _data(data) {}
   explicit TreeNode(E&& data) : _children(), _data(std::move(data)) {}
            TreeNode(TreeNode&& node);
            ~TreeNode();

 // get/set interface
   E* get() {
      return &_data;
   }

   E& operator*() {
      return _data;
   }

   TreeNode<E>& operator[](int index) {
      return _children[index];
   }

   bool           hasChildren() const;
   Children&      getChildren() { return _children; }

   TreeNode<E>*   getChild(int index);
   TreeNode<E>*   find(const E& element);
    
 // query
   size_t count() const {
      int result = 1; // self
      for ( auto& node : _children )
      {
         result += node.count();
      }
      return result;
   }

 // operations
   TreeNode<E>& insert_child(const E& element) {
      _children.emplace_back(element);
      return _children.back();
   }

   TreeNode<E>& insert_child(E&& element) {
      _children.emplace_back(std::move(element));
      return _children.back();
   }
   
private:

   Children       _children;
   E              _data;
};

template <class E>
TreeNode<E>* TreeNode<E>::find(const E& element)
{
   if ( &_data == &element )
      return this;

   for ( auto& child : _children )
   {
      auto pnode = child.find(element);
      if ( pnode )
         return pnode;
   }
   return nullptr;
}

#include "treenode.inl"

#endif
