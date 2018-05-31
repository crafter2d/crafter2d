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
ListIterator<E>::ListIterator()
{
   ASSERT_MSG(false, "Should not be used!");
}

template <class E>
ListIterator<E>::ListIterator(List<E>& list, ListNode<E>* pnode):
   mpList(&list),
   mpNode(pnode)
{
}

template <class E>
ListIterator<E>& ListIterator<E>::operator=(const ListIterator& that)
{
   mpList = that.mpList;
   mpNode = that.mpNode;
   return *this;
}

template <class E>
void ListIterator<E>::operator++()
{
   if ( mpNode != nullptr )
   {
      mpNode = mpNode->nextptr();
   }
}

template <class E>
void ListIterator<E>::reset()
{
   mpNode = mpList->_phead;
}

template <class E>
E& ListIterator<E>::item()
{
   ASSERT_PTR(mpNode);
   return mpNode->data();
}

template <class E>
void* ListIterator<E>::key()
{
   return (void*)mpNode;
}

template <class E>
bool ListIterator<E>::isValid() const
{
   return mpNode != nullptr;
}

template <class E>
void ListIterator<E>::remove()
{
   if ( mpNode != nullptr )
   {
      ListNode<E>* ptemp = mpNode->nextptr();
      mpList->remove(*this);

      mpNode = ptemp;
   }
}
