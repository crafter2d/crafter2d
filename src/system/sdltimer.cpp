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
#include "sdltimer.h"

#include <SDL/SDL.h>

#include "timerdataimpl.h"

#define       TIMERDATA(var,data)       TimerDataImpl<float>& var = dynamic_cast<      TimerDataImpl<float>&>(data)
#define CONST_TIMERDATA(var,data) const TimerDataImpl<float>& var = dynamic_cast<const TimerDataImpl<float>&>(data)


SDLTimer::SDLTimer():
   mStart(0.0f)
{
}

TimerData* SDLTimer::createData() const
{
   return new TimerDataImpl<float>();
}

void SDLTimer::releaseData(TimerData*& pdata)
{
   delete pdata;
   pdata = NULL;   
}
   
void SDLTimer::start(TimerData& info)
{
   TIMERDATA(thedata, info);
   thedata.setData(getTicksAsFloat());
}

float SDLTimer::getInterval(const TimerData& info)
{
   CONST_TIMERDATA(thedata, info);
   return getTicksAsFloat() - thedata.getData();
}

float SDLTimer::getTicksAsFloat()
{
   return (float)SDL_GetTicks() / 1000.0f;
}
