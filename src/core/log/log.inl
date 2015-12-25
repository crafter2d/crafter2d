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
#include "core/defines.h"

INLINE Log& Log::operator<< (int i)
{
   file << i;
   flush ();
   return *this;
}
	
INLINE Log& Log::operator<< (char c)
{
   file << c;
   flush ();
   return *this;
}
	
INLINE Log& Log::operator<< (unsigned char u)
{
   file << u;
   flush ();
   return *this;
}
	
INLINE Log& Log::operator<< (const char* s)
{
   if ( s != NULL )
   {
      file << s;
      flush ();
   }
   return *this;
}

INLINE Log& Log::operator<< (float f) 
{
   file << f;
   flush ();
   return *this; 
}

INLINE Log& Log::operator<< (const std::string& str)
{
   file << str.c_str();
   flush();
   return *this;
}

INLINE Log& Log::put(char c)
{ 
   file << c;
   return *this;
}

INLINE Log& Log::put(char* str) 
{
   file << str; 
   return *this;
}

INLINE void Log::flush ()
{
   file.flush();
}
