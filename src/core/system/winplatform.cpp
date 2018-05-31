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
#include <DbgHelp.h>

#include "core/log/log.h"
#include "core/string/string.h"

#include "wintimer.h"

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
   _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

   ExInfo.ThreadId = ::GetCurrentThreadId();
   ExInfo.ExceptionPointers = ExceptionInfo;
   ExInfo.ClientPointers = TRUE;

   HANDLE hFile = ::CreateFile(L"minidump.dmp", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
   if ( hFile != INVALID_HANDLE_VALUE )
   {
      MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, nullptr, nullptr);

      ::CloseHandle(hFile);
   }
   
   return EXCEPTION_EXECUTE_HANDLER;
#else
   return 0;
#endif 
}

WinPlatform::WinPlatform():
   Platform(),
   mTimer()
{
}

WinPlatform::~WinPlatform()
{
}

c2d::Platform::OS WinPlatform::getOS() const
{
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
   return Platform::eWindows;
#else
   return Platform::eWindowsRT;
#endif
}

Timer& WinPlatform::getTimer()
{
   return mTimer;
}

void WinPlatform::initialize()
{
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
   SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
#endif
}

void* WinPlatform::loadModule(const String& name)
{
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
   HMODULE handle = LoadLibraryEx(name.c_str(), nullptr, 0);
#else
   HMODULE handle = LoadPackagedLibrary(name.c_str(), 0);
#endif

   if ( handle == nullptr )
   {
      DWORD error = GetLastError();
      Log::getInstance().error("Could not load library %s (error %d)", name.toUtf8().c_str(), error);
   }
   return handle;
}

void WinPlatform::freeModule(void* pmodule)
{
   FreeLibrary((HMODULE)pmodule);
}

void* WinPlatform::getFunctionAddress(void* pmodule, const String& name)
{
   std::string uname = name.toUtf8();
   return GetProcAddress((HMODULE)pmodule, uname.c_str());
}
