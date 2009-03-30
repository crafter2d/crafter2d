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

#include <SDL/SDL.h>
#include <string>

class Timer;

class ProfilerItem
{
public:
   explicit ProfilerItem(const std::string& name);
   ~ProfilerItem();

   const std::string&   getName() const;
   Uint32               getStartTime() const;

   void asString(char line[]);

   void increaseChildSampleTime(double amount);

   void begin();
   void end();

   void updateHistory(double damping, double elapsedtime);

private:
   void reset();

   std::string mName;
   int         mCalls;
   int         mParents;
   double      mStartTime;
   double      mAccumulator;
   double      mChildenSampleTime;
   double      mAverage;
   double      mMinimum;
   double      mMaximum;
   bool        mActive;

   Timer*   mpTimer;
};

#endif
