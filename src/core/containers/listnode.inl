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

template <class N>
ListNode<N>::ListNode(const N& element, ListNode<N>* pprev, ListNode<N>* pnext):
   _data(element),
   _pprev(pprev),
   _pnext(pnext)
{
}

template <class E>
bool ListNode<E>::hasNext() const
{
   return _pnext != NULL;
}

template <class N>
ListNode<N>& ListNode<N>::next()
{
   return *_pnext;
}

template <class N>
ListNode<N>* ListNode<N>::nextptr()
{
   return _pnext;
}

template <class N>
const ListNode<N>* ListNode<N>::nextptr() const
{
   return _pnext;
}

template <class N>
void ListNode<N>::next(ListNode<N>* pnext)
{
   _pnext = pnext;
}

template <class E>
bool ListNode<E>::hasPrev() const
{
   return _pprev != NULL;
}

template <class N>
ListNode<N>& ListNode<N>::prev()
{
   return *_pprev;
}

template <class N>
ListNode<N>* ListNode<N>::prevptr()
{
   return _pprev;
}

template <class N>
void ListNode<N>::prev(ListNode<N>* pprev)
{
   _pprev = pprev;
}

template <class N>
N& ListNode<N>::data()
{
   return (N&) _data;
}

template <class N>
const N& ListNode<N>::data() const
{
   return (const N&) _data;
}
