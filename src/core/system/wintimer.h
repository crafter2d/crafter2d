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
#ifndef WIN_TIMER_H_
#define WIN_TIMER_H_

#include "timer.h"

struct QueryData;

class WinTimer : public Timer
{
public:
   WinTimer();
   virtual ~WinTimer();

   virtual TimerData*   createData() const;
   virtual void         releaseData(TimerData*& pdata);
   
   virtual void         start(TimerData& info);
   virtual float        getInterval(const TimerData& info);
   virtual float        getTick() const;

private:
   QueryData* mpData;
};

#endif
