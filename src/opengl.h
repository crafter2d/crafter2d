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
#ifndef OPENGL_H
#define OPENGL_H

#ifdef WIN32
#include "shaderlang.h"
#endif

#include "codepath.h"

class VertexBuffer;

/**
@author Jeroen Broekhuizen
\brief Exposes some general functions to create OpenGL objects.

To enable programmers to use an easy way to create OpenGL objects this class was invented. This class gives
the you basic functions to create the objects supported on the graphics card, so it save you from the troubles
of checking whether an extension is supported or not.Make sure to call initialize before any of the other functions.
*/
class OpenGL
{
public:
	static bool          initialize();
   static VertexBuffer* createVertexBuffer();

   static bool          isPathSupported(CodePath::PathType pathtype);
   static CodePath*     createCodePath(CodePath::PathType pathtype);

   static bool          supportsCG();
	static bool          supportsGLSL ();

   static bool          supportsVertexShader(CodePath::PathType pathtype);
	static bool          supportsFragmentShader(CodePath::PathType pathtype);

	static int           glslToASM (char** code);

private:

#ifdef WIN32
	static ShHandle vertexCompiler;
#endif
};

#endif
