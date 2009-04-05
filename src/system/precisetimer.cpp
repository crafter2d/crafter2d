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
#include "precisetimer.h"

#include <windows.h>

#include "timerdataimpl.h"

#define       TIMERDATA1(var,data)       TimerDataImpl<LARGE_INTEGER>& var = dynamic_cast<      TimerDataImpl<LARGE_INTEGER>&>(data)
#define CONST_TIMERDATA1(var,data) const TimerDataImpl<LARGE_INTEGER>& var = dynamic_cast<const TimerDataImpl<LARGE_INTEGER>&>(data)

struct PreciseTimerData
{
   PreciseTimerData();

   LARGE_INTEGER mFreq;
};

PreciseTimerData::PreciseTimerData():
   mFreq()
{
   QueryPerformanceFrequency(&mFreq);
}

PreciseTimer::PreciseTimer():
   Timer(),
   mpData(NULL)
{
   mpData = new PreciseTimerData();
}

PreciseTimer::~PreciseTimer()
{
   delete mpData;
   mpData = NULL;
}

TimerData* PreciseTimer::createData() const
{
   return new TimerDataImpl<LARGE_INTEGER>();
}

void PreciseTimer::releaseData(TimerData*& pdata)
{
   TimerDataImpl<LARGE_INTEGER>* pmydata = dynamic_cast< TimerDataImpl<LARGE_INTEGER>* >(pdata);

   delete pmydata;
   pdata = NULL;
}

void PreciseTimer::start(TimerData& info)
{
   TIMERDATA1(thedata, info);
   LARGE_INTEGER& start = thedata.getData();
   QueryPerformanceCounter(&start);
}

float PreciseTimer::getInterval(const TimerData& info)
{
   LARGE_INTEGER end;
   QueryPerformanceCounter(&end);

   CONST_TIMERDATA1(thedata, info);
   const LARGE_INTEGER& start = thedata.getData();

   return (float)((double)(end.QuadPart - start.QuadPart) / (double)mpData->mFreq.QuadPart);
}