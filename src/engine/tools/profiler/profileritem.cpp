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

#include "core/defines.h"

#include "core/system/timer.h"

#include "profiler.h"

ProfilerItem::ProfilerItem(const std::string& name):
   mName(name),
   mpTimerData(TIMER.createData()),
   mCalls(0),
   mParents(0),
   mStartTime(0),
   mAccumulator(0),
   mChildenSampleTime(0),
   mAverage(0.0f),
   mMinimum(0.0f),
   mMaximum(0.0f),
   mActive(false)
{
   ASSERT_PTR(mpTimerData)
}

ProfilerItem::~ProfilerItem()
{
   TIMER.releaseData(mpTimerData);
}

const std::string& ProfilerItem::getName() const
{
   return mName;
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

void ProfilerItem::increaseChildSampleTime(float amount)
{
   mChildenSampleTime += amount;
}

void ProfilerItem::begin()
{
   ASSERT(!mActive)

   Profiler::getInstance().push(*this);

   mCalls++;
   mActive = true;

   Timer::getInstance().start(*mpTimerData);
}

void ProfilerItem::end()
{
   const float duration = TIMER.getInterval(*mpTimerData);

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

void ProfilerItem::updateHistory(float damping, float elapsedtime)
{
   ASSERT(!mActive)

   if ( elapsedtime > 0 )
   {
      float sampletime  = mAccumulator - mChildenSampleTime;
      float percenttime = (sampletime / elapsedtime) * 100.0f;

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
