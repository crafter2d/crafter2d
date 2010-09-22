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
#ifndef TIMER_H_
#define TIMER_H_

#define TIMER Timer::getInstance()

class TimerData;

class Timer
{
public:
   static Timer&  getInstance();

   virtual ~Timer() = 0;

   virtual TimerData*   createData() const = 0;
   virtual void         releaseData(TimerData*& pdata) = 0;
   
   virtual void         start(TimerData& info) = 0;
   virtual float        getInterval(const TimerData& info) = 0;
   virtual float        getTick() const;

protected:
   Timer();
};

#endif
