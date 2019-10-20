/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "linuxtimer.h"

#include <SDL2/SDL_timer.h>

#include "timerdataimpl.h"

#define TIMERDATA(var,data)   TimerDataImpl<Uint32>& var = static_cast<TimerDataImpl<Uint32>&>(data)
#define CTIMERDATA(var,data)  const TimerDataImpl<Uint32>& var = static_cast<const TimerDataImpl<Uint32>&>(data)

LinuxTimer::LinuxTimer():
   Timer()
{

}

// - Overloads

TimerData* LinuxTimer::createData() const
{
   return new TimerDataImpl<Uint32>();
}

void LinuxTimer::releaseData(TimerData*& pdata)
{
   TimerDataImpl<Uint32>* pmydata = static_cast< TimerDataImpl<Uint32>* >(pdata);

   delete pmydata;
   pdata = NULL;
}

void LinuxTimer::start(TimerData& info)
{
   TIMERDATA(data, info);
   Uint32& start = data.getData();
   start = SDL_GetTicks();
}

float LinuxTimer::getInterval(const TimerData& info)
{
   CTIMERDATA(data, info);
   return (SDL_GetTicks() - data.getData()) / 1000.0f;
}

float LinuxTimer::getTick() const
{
   return SDL_GetTicks() / 1000;
}
