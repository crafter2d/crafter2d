/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#include "core/log/log.h"
#ifndef JENGINE_INLINE
#  include "log.inl"
#endif

#include <stdarg.h>

static const std::string SInfo    = "Info";
static const std::string SWarning = "Warning";
static const std::string SError   = "Error";

// static 
Log& Log::getInstance()
{
   static Log log;
   return log;
}

Log::Log()
{
	file.open ("log.txt");
}

Log::~Log()
{
   try
   {
      if (file.is_open())
	      file.close ();
   }
   catch(...)
   {
   }
}

void Log::info(const char* msg, ...)
{
   ASSERT_PTR(msg);

   char text[256];

   va_list ap;
	va_start (ap, msg);
	vsprintf (text, msg, ap);
	va_end (ap);

   writeInfo(SInfo, text);
}

void Log::warning(const char* msg, ...)
{
   ASSERT_PTR(msg);

   char text[256];

   va_list ap;
	va_start (ap, msg);
	vsprintf (text, msg, ap);
	va_end (ap);

   writeInfo(SWarning, text);
}

void Log::error(const char* msg, ...)
{
   ASSERT_PTR(msg);

   char text[256];

   va_list ap;
	va_start (ap, msg);
	vsprintf (text, msg, ap);
	va_end (ap);

   writeInfo(SError, text);
}

void Log::writeInfo(const std::string& type, const char* msg)
{
   *this << type.c_str() << " - " << msg << '\n';
}
