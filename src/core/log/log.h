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
#ifndef _LOG_H_
#define _LOG_H_

#include "core/core_base.h"

#include <fstream>

class CORE_API Log
{
public:
   static Log& getInstance();

   enum Type { eInfo, eWarning, eError };

	      ~Log();

   Log&  operator<< (int i);
	Log&  operator<< (char c);
	Log&  operator<< (unsigned char u);
	Log&  operator<< (const char* s);
   Log&  operator<< (float f);

   Log&  put(char c);
	Log&  put(char* str);

   void  info(const char* msg, ...);
   void  warning(const char* msg, ...);
   void  error(const char* msg, ...);

	void  flush();

private:
   Log();

   void writeInfo(const std::string& type, const char* msg);

   std::ofstream file;
};

#ifdef JENGINE_INLINE
#  include "log.inl"
#endif

#endif 
