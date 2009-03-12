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
#include "../defines.h"

template <class E>
ListIterator<E>::ListIterator()
{
   ASSERT_MSG(false, "Should not be used!");
}

template <class E>
ListIterator<E>::ListIterator(List<E>& list):
   _list(list),
   _pnode(NULL)
{
   reset();
}

template <class E>
void ListIterator<E>::reset()
{
   _pnode = _list.getHead();
}

template <class E>
void ListIterator<E>::operator++()
{
   _pnode = _pnode->nextptr();
}

template <class E>
bool ListIterator<E>::isValid()
{
   return _pnode != NULL;
}

template <class E>
E& ListIterator<E>::item()
{
   ASSERT_PTR(_pnode);
   return _pnode->data();
}

template <class E>
void* ListIterator<E>::key()
{
   return (void*)_pnode;
}
