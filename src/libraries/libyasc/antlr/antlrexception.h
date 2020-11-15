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
#ifndef ANTLR_EXCEPTION_H
#define ANTLR_EXCEPTION_H

#include "libyasc/exceptions/compileexception.h"

class AntlrException : public CompileException
{
public:
            AntlrException();
   explicit AntlrException(const String& msg, int line = -1, int charpos = -1);

 // get/set
   const String& getMessage() const;
   int                getCharPosition() const;

 // query
   virtual String asString() const;

private:
   String mMessage;
   int         mCharPos;
};

#ifdef JENGINE_INLINE
#  include "antlrexception.inl"
#endif

#endif // ANTLR_EXCEPTION_H
