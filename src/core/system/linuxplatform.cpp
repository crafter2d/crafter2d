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
#include "linuxplatform.h"

#include "core/defines.h"

#include "linuxtimer.h"

LinuxPlatform::LinuxPlatform():
   Platform(),
   mpTimer(new LinuxTimer())
{
}

LinuxPlatform::~LinuxPlatform()
{
  delete mpTimer;
}
// - Query

Platform::OS LinuxPlatform::getOS() const
{
   return Platform::eLinux;
}

Timer& LinuxPlatform::getTimer()
{
   ASSERT_PTR(mpTimer);
   return *mpTimer;
}

// - Operations

void LinuxPlatform::initialize()
{
}

void* LinuxPlatform::loadModule(const String& name)
{
   return NULL;
}

void LinuxPlatform::freeModule(void* pmodule)
{
}

void* LinuxPlatform::getFunctionAddress(void* module, const String& name)
{
   return NULL;
}
