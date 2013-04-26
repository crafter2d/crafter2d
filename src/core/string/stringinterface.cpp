/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "stringinterface.h"

#include "string.h"

unsigned long sCrcTable[256];
bool          sCrcTableComputed = false;

void buildCrcTable()
{
   unsigned long c;
   int n, k;
   
   for ( n = 0; n < 256; n++ )
   {
      c = (unsigned long) n;
      for (k = 0; k < 8; k++)
      {
         if (c & 1)
            c = 0x04C11DB7 ^ (c >> 1);
         else
            c = c >> 1;
      }
      sCrcTable[n] = c;
   }

   sCrcTableComputed = true;
}

unsigned long StringInterface::crc(const String& text)
{
   if ( !sCrcTableComputed )
   {
      buildCrcTable();
   }

   unsigned long result = 0xffffffffL;
   for ( int index = 0; index < text.length(); ++index )
   {
      char c = text[index];
      result = sCrcTable[(result ^ c) & 0xff] ^ (result >> 8);
   }

	return result;
}

std::vector<String> StringInterface::tokenize(const String& text, char separator)
{
   std::vector<String> result;
   String value;
   for ( int index = 0; index < text.length(); ++index )
   {
      if ( text[index] == separator )
      {
         result.push_back(value.trim());
         value = "";
      }
      else
      {
         value += text[index];
      }
   }
   if ( !value.isEmpty() )
   {
      result.push_back(value.trim());
   }
   return result;
}
