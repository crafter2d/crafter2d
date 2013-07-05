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
#include "shader.h"
#ifndef JENGINE_INLINE
#  include "shader.inl"
#endif

#include <string.h>
#include <GL/glu.h>

#include "core/smartptr/autoptr.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"
#include "core/log/log.h"

using namespace Graphics;

/*!
    \fn Shader::Shader ()
	 \brief Constructor of the Shader class, for initialization of the member variables.
 */
Shader::Shader():
   mShader(0)
{
}

/*!
    \fn Shader::~Shader ()
	 \brief Destructor of the Shader class. It releases the shader if not done so already.
 */
Shader::~Shader()
{
	release ();
}

/*!
    \fn Shader::compile(const String& filename)
	 \brief Tries to compile the shader to native assembly language. Make sure you have first loaded in a shader.
	 \retval true when compilation was successfull
	 \retval false if there was no shader, or the shader had an error.
 */
bool Shader::compile(const String& filename)
{
   int length = 0;
   GLchar* code = load(filename, length);
   if( code != NULL )
   {
      bool suc = compile( code, length );
      delete[] code;
      return suc;
   }
   else
      return false;
}

bool Shader::compile( const char* source, int length )
{
	assert(mShader != 0 && "Shader.compile: shader object is not initialized. make sure you use the Vertex- or FragmentShader class.");

	// load in the shader and compile it
	glShaderSource(mShader, 1, (const GLchar**)&source, &length);
	glCompileShader(mShader);

	// see if the code was compiled successfully
   GLint compiled;
   glGetShaderiv(mShader, GL_COMPILE_STATUS, &compiled);
	if ( compiled != GL_TRUE )
   {
		GLint maxLength = 0;
      glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar *log = new GLcharARB[maxLength];
		glGetShaderInfoLog(mShader, maxLength, &length, log);
      Log::getInstance().info("Compiler log: %s", log);
		delete[] log;
		return false;
	}
	return true;
}

/// \fn Shader::load (const String& filename)
/// \brief Loads in a shader from a file.
/// \param[in] filename name of the shader file
/// \param     length   set to length of the code
/// \returns pointer to the source code loaded, user must release memory.
GLcharARB* Shader::load (const String& filename, int& length)
{
   AutoPtr<File> pfile = FileSystem::getInstance().open(filename, File::ERead | File::EBinary);
   if ( !pfile.hasPointer() )
   {
      Log::getInstance().error("Shader.load: Can not open shader file: %s", filename);
		return false;
	}

	// determine file length
	length = pfile->size();

	// read in the complete data
	GLchar* source = new GLchar[length+1];
	if ( source == NULL )
   {
      Log::getInstance().error("Shader.load: No memory available for source.");
		return false;
	}

	memset(source, 0, length+1);
	pfile->read(source, length);
	return source;
}

/*!
    \fn Shader::release()
	 \brief When the shader object is not neccessary anymore, this function can be called to release memory. This is done
	 automatically by the ShaderObject::link and destructor of this class.
	 \return Nothing
 */
void Shader::release()
{
	if (mShader != 0)
   {
		glDeleteShader(mShader);
		mShader = 0;
	}
}

/*!
    \fn VertexShader::VertexShader()
	 \brief Creates a vertex shader object.
 */
VertexShader::VertexShader()
{
	handle(glCreateShader(GL_VERTEX_SHADER));
	if ( handle() == NULL )
   {
      Log::getInstance().error("VertexShader: Can not create shader object.");
   }
}

/*!
    \fn FragmentShader::FragmentShader()
	 \brief Creates a fragment shader object. Note that fragment shaders are only available on the latests graphic cards.
 */
FragmentShader::FragmentShader()
{
	handle(glCreateShader(GL_FRAGMENT_SHADER));
	if ( handle() == NULL )
   {
      Log::getInstance().error("FragmentShader: Can not create shader object.");
   }
}
