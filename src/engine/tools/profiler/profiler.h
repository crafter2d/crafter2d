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
#ifndef PROFILER_H_
#define PROFILER_H_

#include <map>
#include <string>
#include <stack>

class ProfilerItem;
class TimerData;

class Profiler
{
public:
   typedef std::map<std::string, ProfilerItem*> ItemMap;
   typedef std::stack<ProfilerItem*>            CallStack;

 // singleton instance
   static Profiler& getInstance();

   ~Profiler();

   bool           isCurrent(const ProfilerItem& item) const;

   bool           hasParent() const;
   ProfilerItem&  getParent();
   int            getParentCount() const;

   void push(ProfilerItem& item);
   void pop();

   void begin();
   void end();

 // drawing
   void render();

 // searching
   ProfilerItem*  findOrCreateItem(const std::string& name);

private:
   static Profiler mInstance;

   Profiler();

   TimerData&  getTimerData();

   bool contains(const std::string& name) const;

   ItemMap        mItems;
   CallStack      mCallstack;
   TimerData*     mpTimerData;
   float          mDamping;
};

#endif
