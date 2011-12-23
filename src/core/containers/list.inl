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
#include "core/defines.h"

template <class E>
List<E>::List():
   _phead(NULL),
   _ptail(NULL),
   _size(0)
{
}

template <class E>
List<E>::~List()
{
}

template <class E>
ListNode<E>* List<E>::getHead() const
{
   return _phead;
}

template <class E>
ListNode<E>* List<E>::getTail() const
{
   return _ptail;
}

template <class E>
ListNode<E>* List<E>::get(int index)
{
   if ( index < size() )
   {
      ListNode<E>* ppos = _phead;
      for ( int i = 0; i < index; ++i )
      {
         ppos = ppos->nextptr();
      }
      return ppos;
   }
   else
      return NULL;
}

template <class E>
void List<E>::addFront(E* pelement)
{
   if ( _phead == NULL )
   {
      _ptail = _phead = new ListNode<E>(pelement, NULL, NULL);
   }
   else
   {
      _phead->prev( new ListNode<E>(pelement, NULL, _phead) );
      _phead = _phead->prevptr();
   }
}

template <class E>
void List<E>::addTail(E* pelement)
{
   if ( _ptail == NULL )
   {
      _ptail = _phead = new ListNode<E>(pelement, NULL, NULL);
   }
   else
   {
      _ptail->next( new ListNode<E>(pelement, _ptail, NULL) );
      _ptail = _ptail->nextptr();
   }
   
   ++_size;
}

template <class E>
void List<E>::insert(Iterator<E>& it, E* pelement)
{
   ListNode<E>* node = (ListNode<E>*)it.key();
   ASSERT_PTR(node);
   
   if ( _ptail == NULL )
   {
      _phead = _ptail = new ListNode<E>(pelement, NULL, NULL);
   }
   else if ( !node->hasPrev() )
   {
      addFront(pelement);
   }
   else if ( !node->hasNext() )
   {
      addTail(pelement);
   }
   else
   {
      ASSERT(node->hasPrev() && node->hasNext());
      
      ListNode<E>* pnode = new ListNode<E>(pelement, node->prevptr(), node->nextptr());
      node->prevptr()->next(pnode);
      node->nextptr()->prev(pnode);
   }
   
   ++_size;
}

template <class E>
void List<E>::clear()
{
   ListNode<E>* pcurrent = _phead;
   while ( pcurrent != NULL )
   {
      ListNode<E>* pnext = pcurrent->nextptr();
      delete pcurrent;
      pcurrent = pnext;
   }

   _phead = _ptail = NULL;
   _size = 0;
}

// - Query

template <class E>
int List<E>::size() const
{
   return _size;
}

template <class E>
bool List<E>::isEmpty() const
{
   return _size == 0;
}
