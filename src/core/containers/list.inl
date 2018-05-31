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

#include "listiterator.h"
#include "listconstiterator.h"

template <class E>
List<E>::List():
   _phead(nullptr),
   _ptail(nullptr),
   _size(0)
{
}

template <class E>
List<E>::~List()
{
   clear();
}

// - Query

template <class E>
ListIterator<E> List<E>::getFront()
{
   return ListIterator<E>(*this, _phead);
}

template <class E>
ListConstIterator<E> List<E>::getFront() const
{
   return ListConstIterator<E>(*this, _phead);
}

template <class E>
ListIterator<E> List<E>::getTail()
{
   return ListIterator<E>(*this, _ptail);
}

template <class E>
ListConstIterator<E> List<E>::getTail() const
{
   return ListConstIterator<E>(*this, _ptail);
}

template <class E>
ListIterator<E> List<E>::get(int index)
{
   if ( index < size() )
   {
      ListNode<E>* ppos = _phead;
      for ( int i = 0; i < index; ++i )
      {
         ppos = ppos->nextptr();
      }
      return ListIterator<E>(*this, ppos);
   }
   
   return ListIterator<E>();
}

// - Maintenance

template <class E>
void List<E>::addFront(const E& element)
{
   if ( _phead == nullptr )
   {
      _ptail = _phead = new ListNode<E>(element, nullptr, nullptr);
   }
   else
   {
      _phead->prev( new ListNode<E>(element, nullptr, _phead) );
      _phead = _phead->prevptr();
   }

   ++_size;
}

template <class E>
void List<E>::addTail(const E& element)
{
   if ( _ptail == nullptr )
   {
      _ptail = _phead = new ListNode<E>(element, nullptr, nullptr);
   }
   else
   {
      _ptail->next( new ListNode<E>(element, _ptail, nullptr) );
      _ptail = _ptail->nextptr();
   }
   
   ++_size;
}

/// \fn List<E>::insert(Iterator<E>& it, E& element)
/// \brief Inserts element in the list. It is inserted in front of the iterator.
template <class E>
void List<E>::insert(Iterator<E>& it, E& element)
{
   if ( it.isValid() )
   {
      ListNode<E>* pnode = static_cast<ListNode<E>*>(it.key());
      ASSERT_PTR(pnode);

      if ( _phead == nullptr )
      {
         _phead = _ptail = new ListNode<E>(element, nullptr, nullptr);
      }
      else if ( !pnode->hasPrev() )
      {
         addFront(element);
      }
      else
      {
         ASSERT(pnode->hasPrev());
      
         ListNode<E>* pnewnode = new ListNode<E>(element, pnode->prevptr(), pnode);
         pnode->prevptr()->next(pnewnode);
         pnode->prev(pnewnode);
      }
   
      ++_size;
   }
}

template <class E>
void List<E>::clear()
{
   ListNode<E>* pcurrent = _phead;
   while ( pcurrent != nullptr )
   {
      ListNode<E>* pnext = pcurrent->nextptr();
      delete pcurrent;
      pcurrent = pnext;
   }

   _phead = _ptail = nullptr;
   _size = 0;
}

template <class E>
void List<E>::remove(Iterator<E>& it)
{
   ListNode<E>* pnode = static_cast<ListNode<E>*>(it.key());
   if ( _phead == _ptail )
   {
      _phead = _ptail = nullptr;
   }
   else
   {
      if ( pnode->hasPrev() )
      {
         pnode->prev().next(pnode->nextptr());
      }
      else
      {
         _phead = pnode->nextptr();
      }

      if ( pnode->hasNext() )
      {
         pnode->next().prev(pnode->prevptr());
      }
      else
      {
         _ptail = pnode->prevptr();
      }
   }

   delete pnode;
   _size--;
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
