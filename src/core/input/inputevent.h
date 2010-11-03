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
#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include "core/core_base.h"

class CORE_API InputEvent
{
public:
 // enums
   enum KeyModifiers {
      eNone  = 0,
      eShift = 1,
      eCtrl  = 2,
      eAlt   = 4
   };

   explicit InputEvent(int keymodifiers);
   virtual ~InputEvent() = 0;

 // query
   bool isShiftDown() const;
   bool isCtrlDown() const;
   bool isAltDown() const;

   bool isConsumed() const;

 // operations
   void consume() const;
   
private:
   int  mKeyModifiers;
   bool mConsumed;
};

#ifdef JENGINE_INLINE
#  include "inputevent.inl"
#endif

#endif
