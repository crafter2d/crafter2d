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
class Listeners
{
public:
   using Container = std::vector<T*>;
   typedef typename Container::iterator iterator;
   typedef typename Container::const_iterator const_iterator;

   Listeners() : mListeners() {}
   virtual ~Listeners() {}

   iterator begin() {
      return mListeners.begin();
   }
   iterator end() {
      return mListeners.end();
   }

   const_iterator begin() const {
      return mListeners.begin();
   }
   const_iterator end() const {
      return mListeners.end();
   }

  // maintenance
   void addListener(T& listener) { 
      mListeners.push_back(&listener);
   }
   void removeListener(const T& listener) {
      auto it = std::remove(mListeners.begin(), mListeners.end(), &listener);
      mListeners.erase(it);
   }

private:

   Container mListeners;
};

#endif
