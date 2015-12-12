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
#ifndef SORTED_LIST_H_
#define SORTED_LIST_H_

#include <vector>

template <class E>
class SortedList
{
   using List = std::vector<E>;
public:
   using iterator = typename List::iterator;

 // types
   typedef int(*CompareFnc)(const E& left, const E& right);

   SortedList();
   ~SortedList() {}
   
   iterator begin() {
      return mList.begin();
   }
   
   iterator end() {
      return mList.end();
   }

 // get/set interface
   void setCompareFnc(CompareFnc comparefunction);

 // operations
   void add(E& element) {
      mList.push_back(element);
   }
   
   void erase(iterator first, iterator last) {
      mList.erase(first, last);
   }
   
   void sort();

private:
   bool ensureSorted();
 
 // data
   List        mList;
   CompareFnc  mpCompareFnc;
};

#include "sortedlist.inl"

#endif
