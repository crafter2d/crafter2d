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
#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "core/core_base.h"

#include "core/string/char.h"

class String;
class Timer;

namespace c2d
{
   class CORE_API Platform
   {
   public:
      static Platform&  getInstance();

      enum OS
      {
         eWindows,
         eWindowsRT,
         eWindowsPhone,
         eLinux,
      };

      virtual ~Platform();

      // query
      virtual OS getOS() const = 0;
      virtual Timer& getTimer() = 0;

      // operations
      virtual void  initialize() = 0;
      virtual void* loadModule(const String& name) = 0;
      virtual void  freeModule(void* pmodule) = 0;
      virtual void* getFunctionAddress(void* module, const String& name) = 0;
      virtual UChar preferedSlash() = 0;

   protected:
      Platform();

   private:
      static Platform* spPlatform;
   };
}

#endif
