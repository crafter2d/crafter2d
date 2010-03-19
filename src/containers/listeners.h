/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef LISTENERS_H
#define LISTENERS_H

#include <vector>
#include <algorithm>

template <class T>
class Listeners : protected std::vector<T*>
{
public:
   typedef std::vector<T*> ListenersImp;

   Listeners();
   virtual ~Listeners();

  // maintenance
   void addListener(T& listener);
   void removeListener(const T& listener);
};

template<class T>
Listeners<T>::Listeners():
   ListenersImp()
{
}

template<class T>
Listeners<T>::~Listeners()
{
}

// maintenance

template<class T>
void Listeners<T>::addListener(T& listener)
{
   push_back(&listener);
}

template<class T>
void Listeners<T>::removeListener(const T& listener)
{
   iterator it = std::find(begin(), end(), &listener);
   if ( it != end() )
   {
      erase(it);
   }
}

#endif
