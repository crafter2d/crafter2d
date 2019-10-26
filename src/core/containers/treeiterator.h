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
#ifndef TREE_ITERATOR_H_
#define TREE_ITERATOR_H_

#include <iterator>

#include "treenode.h"
#include "treetraversal.h"

template <class E> class Tree;

/**
@author Jeroen Broekhuizen
*/
template <class E, class A = std::allocator<E>>
class TreeIterator final
{
public:
   typedef typename A::difference_type difference_type;
   typedef typename A::value_type value_type;
   typedef typename A::reference reference;
   typedef typename A::pointer pointer;
   typedef std::forward_iterator_tag iterator_category;

   TreeIterator(Tree<E>& tree, ITreeTraversal<E>* ptraverser) : mpTree(&tree), mpTraverser(ptraverser) {}
   TreeIterator(TreeIterator&& it): mpTree(that.mpTree), mpTraverser(that.mpTraverser) {
      that.mpTraverser = nullptr;
   }
   ~TreeIterator() { delete mpTraverser; };

   TreeIterator& operator=(TreeIterator&& it) {
      if ( this != &it )
      {
         mpTree = it.mpTree;
         mpTraverser = it.mpTraverser;
         it.mpTraverser = nullptr;
      }
      return *this;
   }
   bool operator==(const TreeIterator<E>& it) const {
      return mpTree == it.mpTree && node() == it.node();
   }
   bool operator!=(const TreeIterator<E>& it) const {
      return mpTree == it.mpTree && node() == it.node();
   }

   reference operator*() {
      return mpTraverser->element();
   }

   pointer operator->() {
      return &mpTraverser->element();
   }

   TreeIterator& operator++()
   {
      mpTraverser->traverse();
      return *this;
   }
   
private:
   template<class Element> friend class Tree;

   TreeNode<E>* node() {
      return mpTraverser ? mpTraverser->node() : nullptr;
   }

   const TreeNode<E>* node() const {
      return mpTraverser ? mpTraverser->node() : nullptr;
   }

   Tree<E>*           mpTree;
   ITreeTraversal<E>* mpTraverser;
};

#endif
