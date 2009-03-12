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
#include "console.h"
#include "opengl.h"
#include "vertexprogram.h"

/// \fn VertexProgram::VertexProgram()
/// \brief Creates a vertex program
VertexProgram::VertexProgram()
{
}

VertexProgram::~VertexProgram()
{
   release();
}

void VertexProgram::release ()
{
   if( program != 0 ) {
	   glDeleteProgramsARB (1, &program);
      program = 0;
   }

   if( source != 0 ) {
      delete[] source;
      source = 0;
      length = 0;
   }
}

bool VertexProgram::compile(const char* filename)
{
   std::ifstream file (filename, std::ios::binary );
	if (!file.is_open ()) {
      Console::getInstance().printf("VertexProgram.compile: Can not open vertex assembly file '%s'", filename);
		return false;
	}

	// determine file length
	file.seekg (0, std::ios::end);
	length = file.tellg();
   file.seekg (0, std::ios::beg);
	
	// read in the complete data
	GLcharARB* code = new GLcharARB[length+1];
	if (!source) {
      Console::getInstance().print("VertexProgram.compile: No memory available for vertex program code.");
		return false;
	}

	memset (code, 0, length+1);
	file.read (code, length);
	file.close ();

   // perform the actual code
   bool success = compile( code, length );
   delete[] code;

   return success;
}

bool VertexProgram::compile(const char* code, int length)
{
   save( code, length );

	// see if we must convert the code to ASM
   if ( !OpenGL::supportsGLSL() && strstr (source, "!!ARBvp1.0") == NULL )
   {
		if ((length = OpenGL::glslToASM (&source)) == -1)
			return false;
	}

   glGenProgramsARB (1, &program);
   if( program == 0 ) {
      Console::getInstance().print( "VertexProgram.compile(): Can not create a program." );
      return false;
   }

	glBindProgramARB (GL_VERTEX_PROGRAM_ARB, program);
	glProgramStringARB (GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, length, source);
	
	if (glGetError() == GL_INVALID_OPERATION) {
		GLint pos;
		// get the error message and position
		glGetIntegerv ( GL_PROGRAM_ERROR_POSITION_ARB, &pos );
		const GLubyte* msg = glGetString( GL_PROGRAM_ERROR_STRING_ARB );

      Console& console = Console::getInstance();
      console.printf("VertexProgram.compile: error during compilation '%s'", (char*)msg);
		console.printf("Shader (len=%d)", length);

      release();
      return false;
	}
	
   return true;
}

bool VertexProgram::save( const char* code, int len )
{
   length = len;
   source = new GLcharARB[length+1];
	if (source != NULL)
   {
	   strcpy (source, code);
   }

   return (source != 0);
}

void VertexProgram::enable() const
{
	glBindProgramARB (GL_VERTEX_PROGRAM_ARB, program);
	glEnable(GL_VERTEX_PROGRAM_ARB);
}

void VertexProgram::disable() const
{
    glDisable(GL_VERTEX_PROGRAM_ARB);
}

GLint VertexProgram::getUniformLocation (const char* name) const
{
	const char* pos = strstr (source, name);
	if (pos) {
		std::string num;
		// we assume here that it is a PARAM with program.local[n]
		while (*pos != '[') ++pos;
		while (*pos != ']') {
			num += *pos;
			++pos;
		}
		// convert the n to a number
		return (GLint)atoi (num.c_str());
	}
	return -1;
}
