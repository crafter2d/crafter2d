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
#include "winplatform.h"

#include <Windows.h>

#include "core/string/string.h"

#include "wintimer.h"

WinPlatform::WinPlatform():
   Platform()
{
}

WinPlatform::~WinPlatform()
{
}

Timer* WinPlatform::createTimer()
{
   return new WinTimer();
}

void* WinPlatform::loadModule(const String& name)
{
   LPCWSTR pname = (LPCWSTR)name.getBuffer();
   return ::LoadLibraryW(pname);
}

void* WinPlatform::getFunctionAddress(void* pmodule, const String& name)
{
   int len = 0;
   const char* pname = name.toUtf8(len);
   void* pfnc = ::GetProcAddress((HMODULE)pmodule, pname);
   delete[] pname;
   return pfnc;
}
