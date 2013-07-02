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
   Platform(),
   mTimer()
{
}

WinPlatform::~WinPlatform()
{
}

Timer& WinPlatform::getTimer()
{
   return mTimer;
}

void* WinPlatform::loadModule(const String& name)
{
   std::wstring uname = name.toUtf16();
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
   return LoadLibraryEx(uname.c_str(), NULL, 0);
#else
   return LoadPackagedLibrary(uname.c_str(), 0);
#endif
}

void* WinPlatform::getFunctionAddress(void* pmodule, const String& name)
{
   std::string uname = name.toUtf8();
   return GetProcAddress((HMODULE)pmodule, uname.c_str());
}
