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
   TreeNode(E* pdata, TreeNode<E>* pparent);
   ~TreeNode();

 // get/set interface
   bool           hasParent() const;
   TreeNode<E>&   getParent();

   E&             getData();

   bool           hasChildren() const;
   Children&      getChildren() { return _children; }

   TreeNode<E>*   getChild(int index);

   bool           canExpand() const;
   bool           isExpanded() const;

   int            getDepth() const;

 // operations
   void           add(TreeNode<E>* pchild);
   void           expand();
   void           collapse();

private:
   TreeNode<E>*   _pparent;
   Children       _children;
   E*             _pdata;
   bool           _expanded;
};

#include "treenode.inl"

#endif
