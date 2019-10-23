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

#include "iterator.h"
#include "treenode.h"

template <class E>
class Tree
{
public:
   Tree();
   virtual ~Tree();

 // get/set interface
   bool                 hasRoot() const;
   const TreeNode<E>&   getRoot() const;
   TreeNode<E>&         getRoot();

 // operations
   void insert(TreeNode<E>* pposition, E& element);
   void insert(Iterator<E>& it, E& element);

   TreeNode<E>* findElement(const E& element);

private:
   TreeNode<E>* findElement(TreeNode<E>& node, const E& element);

   TreeNode<E>* _proot;
};

#include "tree.inl"

#endif
