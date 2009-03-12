/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

/**
@author Jeroen Broekhuizen
*/

template <class E>
class List
{
public:
   List();
   virtual ~List();
   
 // Get/set interface
   ListNode<E>*   getHead() const;
   ListNode<E>*   getTail() const;
   ListNode<E>*   get(int index);

 // Operations
   void  addFront(E& element);
   void  addTail(E& element);
   
   void  insert(Iterator<E>& it, E& element);
   
   int   size() const;

private:

   ListNode<E>*   _phead;
   ListNode<E>*   _ptail;
   int            _size;
};

#include "listnode.inl"
#include "list.inl"

#endif
