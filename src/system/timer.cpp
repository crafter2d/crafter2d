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
#include "timer.h"

#include "../autoptr.h"
#include "../defines.h"

#include "platform.h"

// static 
Timer& Timer::getInstance()
{
   static AutoPtr<Timer> ptimer(Platform::getInstance().createTimer());
   return *ptimer;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}

TimerData* Timer::createData() const
{
   PURE_VIRTUAL
   return NULL;
}

void Timer::releaseData(TimerData*& pdata)
{
   PURE_VIRTUAL
}
   
void Timer::start(TimerData& info)
{
   PURE_VIRTUAL
}

float Timer::getInterval(const TimerData& info)
{
   PURE_VIRTUAL
   return 0;
}

float Timer::getTick() const
{
   PURE_VIRTUAL
   return 0;
}
