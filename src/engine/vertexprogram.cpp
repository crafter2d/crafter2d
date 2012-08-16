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
#include "vertexprogram.h"

#include <cstring>
#include <cstdlib>

#include "core/log/log.h"
#include "core/string/string.h"
#include "core/conv/numberconverter.h"
#include "core/smartptr/autoptr.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"

#include "opengl.h"

/// \fn VertexProgram::VertexProgram()
/// \brief Creates a vertex program
VertexProgram::VertexProgram():
   program(0),
   source(NULL),
   length(0)
{
}

VertexProgram::~VertexProgram()
{
   release();
}

void VertexProgram::release ()
{
   if ( program != NULL )
   {
      glDeleteProgramsARB (1, &program);
      program = 0;
   }

   if ( source != NULL )
   {
      delete[] source;
      source = 0;
      length = 0;
   }
}

bool VertexProgram::compile(const String& filename)
{
   AutoPtr<File> pfile = FileSystem::getInstance().open(filename, File::ERead | File::EBinary);
   if ( !pfile.hasPointer() )
   {
      Log::getInstance().error("VertexProgram.compile: Can not open vertex assembly file '%s'", filename.getBuffer());
		return false;
	}

	// determine file length
   length = pfile->size();

   bool success = false;
   if ( length > 0 )
   {
	   // read in the complete data
	   char* pcode = new char[length+1];
	   if ( source == NULL )
      {
         Log::getInstance().error("VertexProgram.compile: No memory available for vertex program code.");
		   success = false;
	   }
      else
      {
	      memset(pcode, 0, length+1);
         pfile->read(pcode, length);

         // perform the actual code
         success = compile(pcode, length);
      }

      delete[] pcode;
   }

   return success;
}

bool VertexProgram::compile(const char* code, int length)
{
   save(code, length);

   glGenProgramsARB(1, &program);
   if ( program == 0 )
   {
      Log::getInstance().error( "VertexProgram.compile(): Can not create a program." );
      return false;
   }

   glBindProgramARB (GL_VERTEX_PROGRAM_ARB, program);
   glProgramStringARB (GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, length, source);

   if ( glGetError() == GL_INVALID_OPERATION )
   {
      GLint pos;
      glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &pos);
      const GLubyte* msg = glGetString(GL_PROGRAM_ERROR_STRING_ARB);

      Log::getInstance().error("VertexProgram.compile: error during compilation '%s'", (char*)msg);

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

   return (source != NULL);
}

void VertexProgram::enable() const
{
   glBindProgramARB(GL_VERTEX_PROGRAM_ARB, program);
   glEnable(GL_VERTEX_PROGRAM_ARB);
}

void VertexProgram::disable() const
{
   glDisable(GL_VERTEX_PROGRAM_ARB);
}

GLint VertexProgram::getUniformLocation(const String& name) const
{
   int len;
   const char* pname = name.toUtf8(len);
	const char* pos = strstr(source, pname);
   delete[] pname;
	if ( pos != NULL )
   {
      char num[10] = {};
      int index = 0;

      // we assume here that it is a PARAM with program.local[n]
      pos = strchr(pos, '[');

      while (*pos != ']')
      {
         num[index++] = *pos;
         ++pos;
      }

      // convert the n to a number
      return (GLint)atoi(num);
	}

	return -1;
}
