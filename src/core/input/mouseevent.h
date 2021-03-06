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
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "core/core_base.h"

#include "core/math/point.h"
#include "core/defines.h"

#include "inputevent.h"

namespace Input
{
   class CORE_API MouseEvent : public InputEvent
   {
   public:
     // enums
      enum Button {
         eInvalid   = 0,
         eLeft      = 1,
         eRight     = 2,
         eMiddle    = 4,
         eWheelUp   = 8,
         eWheelDown = 16
      };

      enum EventType {
         ePressed,
         eReleased,
         eMotion
      };

    // construction
      MouseEvent(int buttons, EventType eventtype, int keymodifiers, const c2d::Point& location, const c2d::Point& relative = c2d::Point::zero());
      virtual ~MouseEvent();

    // get/set
            int         getButtons() const   { return mButtons; }
            EventType   getEventType() const { return mEventType; }
      const c2d::Point& getLocation() const  { return mLocation; }
      const c2d::Point& getRelative() const  { return mRelative; }

    // query
      bool isLeftButtonDown() const { return IS_SET(mButtons, eLeft); }
      bool isRightButtonDown() const { return IS_SET(mButtons, eRight); }
      bool isMiddleButtonDown() const { return IS_SET(mButtons, eMiddle); }

      bool isWheelUp() const   { return mButtons == eWheelUp;   }
      bool isWheelDown() const { return mButtons == eWheelDown; }

   private:
     // statics

      int         mButtons;
      EventType   mEventType;
      c2d::Point  mLocation;
      c2d::Point  mRelative;
   };
}

#endif
