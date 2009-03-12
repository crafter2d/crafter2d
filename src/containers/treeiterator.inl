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
TreeIterator<E>::TreeIterator()
{
   ASSERT_MSG(false, "Should not be used!");
}

template <class E>
TreeIterator<E>::TreeIterator(Tree<E>& tree):
   _tree(tree)
{
   //reset();
}

template <class E>
void TreeIterator<E>::reset()
{
}

template <class E>
void TreeIterator<E>::operator++()
{
}

template <class E>
bool TreeIterator<E>::isValid()
{
   return false;
}

template <class E>
E& TreeIterator<E>::item()
{
}

template <class E>
void* TreeIterator<E>::key()
{
   return NULL;
}
