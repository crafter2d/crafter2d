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
#ifndef PROFILER_ITEM_H_
#define PROFILER_ITEM_H_

#include <string>

class TimerData;

class ProfilerItem
{
public:
   explicit ProfilerItem(const std::string& name);
   ~ProfilerItem();

   const std::string&   getName() const;

   std::string toString() const;

   void increaseChildSampleTime(float amount);

   void begin();
   void end();

   void updateHistory(float damping, float elapsedtime);

private:
   void reset();

   std::string mName;
   TimerData*  mpTimerData;
   int         mCalls;
   int         mParents;
   float       mStartTime;
   float       mAccumulator;
   float       mChildenSampleTime;
   float       mAverage;
   float       mMinimum;
   float       mMaximum;
   bool        mActive;
};

#endif
