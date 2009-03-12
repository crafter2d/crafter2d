/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef VERTEXPROGRAM_H_
#define VERTEXPROGRAM_H_

#include <GL/GLee.h>

/// @author Jeroen Broekhuizen
/// \brief ARB vertex program wrapper for backwards compatibility on older hardware.
///
/// When JEngine SSE runs on older hardware (GeForce FX, etc) that do not support
/// fragment shaders and the new shader objects, it will automatically drop back
/// on the ARB vertex program extension. Check the ProgramPath code path for the
/// use of this class.
class VertexProgram
{
public:
	VertexProgram();
   ~VertexProgram();

	void release();

	bool compile( const char* filename);
   bool compile( const char* code, int len );

	void enable() const;
	void disable() const;

	GLint getUniformLocation( const char* name ) const;

private:
   bool save( const char* code, int len );

private:
   GLuint program;
   GLcharARB* source;
   int length;
};

#endif
