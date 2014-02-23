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
#ifndef LIST_H_
#define LIST_H_

#include "iterator.h"
#include "listnode.h"

template <class E> class ListIterator;
template <class E> class ListConstIterator;

/**
@author Jeroen Broekhuizen

The list never owns any of the elements added to it.
*/

template <class E>
class List
{
public:
   List();
   virtual ~List();
   
 // Get/set interface
   ListIterator<E> getFront();
   ListConstIterator<E> getFront() const;
   ListIterator<E> getTail();
   ListConstIterator<E> getTail() const;
   ListIterator<E> get(int index);

 // Query
   int   size() const;
   bool  isEmpty() const;

 // Operations
   void  addFront(const E& element);
   void  addTail(const E& element);
   
   void  insert(Iterator<E>& it, E& element);
   void  clear();

   void  remove(Iterator<E>& it);

private:
   friend class ListIterator<E>;
   friend class ListConstIterator<E>;

   ListNode<E>*   _phead;
   ListNode<E>*   _ptail;
   int            _size;
};

#include "list.inl"

#endif
