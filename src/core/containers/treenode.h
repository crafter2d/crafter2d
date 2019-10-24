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
            TreeNode(): _pparent(nullptr), _children(), _data(), _expanded(true) {}
   explicit TreeNode(E data): _pparent(nullptr), _children(), _data(data), _expanded(true) {}
            TreeNode(TreeNode&& node);
            ~TreeNode();

 // get/set interface
   bool           hasParent() const;
   TreeNode<E>&   getParent();

   E& getData() {
      return _data;
   }

   bool           hasChildren() const;
   Children&      getChildren() { return _children; }

   TreeNode<E>*   getChild(int index);
   TreeNode<E>*   find(const E& element);

   bool           canExpand() const;
   bool           isExpanded() const;

   int            getDepth() const;

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
   TreeNode<E>& insert_child(E& element) {
      auto& node = _children.emplace_back(element);
      node.setParent(this);
      return node;
   }

   void           expand();
   void           collapse();

private:
   void setParent(TreeNode<E>* pparent) {
      _pparent = pparent;
   }

   TreeNode<E>*   _pparent;
   Children       _children;
   E              _data;
   bool           _expanded;
};

template <class E>
TreeNode<E>* TreeNode<E>::find(const E& element)
{
   if ( &_pdata == &element )
      return _pdata;

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
