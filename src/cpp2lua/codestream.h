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
#ifndef CODE_STREAM_H_
#define CODE_STREAM_H_

#include <fstream>

class CodeStream
{
public:
   typedef CodeStream&(*pstreamfnc)(CodeStream&);

   static CodeStream& endl(CodeStream& stream)
   {
      stream << "\n";
      return stream;
   }

   CodeStream():
      mStream()
   {
   }

   CodeStream(const std::string& filename):
      mStream(filename)
   {
   }

   CodeStream& operator<<(const std::string& text) { mStream << text.c_str(); return *this; }
   CodeStream& operator<<(int value) { mStream << value; return *this; }
   CodeStream& operator<<(pstreamfnc fnc) { fnc(*this); return *this; }

private:
   std::ofstream mStream;
};

#endif // CODE_STREAM_H_
