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
#include "profileritem.h"

#include "..\..\defines.h"
#include "..\..\system\timer.h"

#include "profiler.h"

ProfilerItem::ProfilerItem(const std::string& name):
   mName(name),
   mCalls(0),
   mParents(0),
   mStartTime(0),
   mAccumulator(0),
   mChildenSampleTime(0),
   mAverage(0.0f),
   mMinimum(0.0f),
   mMaximum(0.0f),
   mActive(false),
   mpTimer(NULL)
{
   mpTimer = new Timer();
}

ProfilerItem::~ProfilerItem()
{
   if ( mpTimer != NULL )
   {
      delete mpTimer;
      mpTimer = NULL;
   }
}

const std::string& ProfilerItem::getName() const
{
   return mName;
}

Uint32 ProfilerItem::getStartTime() const
{
   return mStartTime;
}

void ProfilerItem::asString(char line[])
{
   char avg[16], min[16], max[16];
   char indentedname[256];
   char name[256];

   sprintf(avg, "%3.1f", mAverage);
   sprintf(min, "%3.1f", mMinimum);
   sprintf(max, "%3.1f", mMaximum);

   strcpy(indentedname, mName.c_str());
   for ( int index = 0; index < mParents; ++index )
   {
      sprintf(name, "   %s", indentedname);
      strcpy(indentedname, name);
   }

   sprintf(line, "%5s, : %5s : %5s : %3s\n", avg, min, max, indentedname);
}

void ProfilerItem::increaseChildSampleTime(double amount)
{
   mChildenSampleTime += amount;
}

void ProfilerItem::begin()
{
   ASSERT(!mActive)

   Profiler::getInstance().push(*this);

   mCalls++;
   mActive = true;
   //mStartTime = SDL_GetTicks();

   mpTimer->begin();
}

void ProfilerItem::end()
{
   const double duration = mpTimer->getInterval();
   //const double endtime = SDL_GetTicks();
   // const double duration = endtime - mStartTime;

   mAccumulator += duration;
   mActive = false;

   Profiler& profiler = Profiler::getInstance();
   ASSERT(profiler.isCurrent(*this))
   profiler.pop();

   mParents = profiler.getParentCount();

   if ( profiler.hasParent() )
   {
      ProfilerItem& parent = profiler.getParent();
      parent.increaseChildSampleTime(duration);
   }
}

void ProfilerItem::updateHistory(double damping, double elapsedtime)
{
   ASSERT(!mActive)

   if ( elapsedtime > 0 )
   {
      double sampletime  = mAccumulator - mChildenSampleTime;
      double percenttime = (sampletime / elapsedtime) * 100.0;

      mAverage = damping * (mAverage - percenttime) + percenttime;

      if ( percenttime < mMinimum )
         mMinimum = percenttime;
      else


      if ( percenttime > mMaximum )
         mMaximum = percenttime;
   }

   reset();
}

void ProfilerItem::reset()
{
   mAccumulator = 0;
   mChildenSampleTime = 0;
   mCalls = 0;
}
