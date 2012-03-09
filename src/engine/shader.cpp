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
#include <iostream>
#include <fstream>
#include <GL/glu.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "core/log/log.h"

#include "opengl.h"

/*!
    \fn Shader::Shader ()
	 \brief Constructor of the Shader class, for initialization of the member variables.
 */
Shader::Shader():
   shader(0)
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
    \fn Shader::compile()
	 \brief Tries to compile the shader to native assembly language. Make sure you have first loaded in a shader.
	 \retval true when compilation was successfull
	 \retval false if there was no shader, or the shader had an error.
 */
bool Shader::compile( const char* filename )
{
   int length = 0;
   GLcharARB* code = load( filename, length );
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
	assert( shader != 0 && "Shader.compile: shader object is not initialized. make sure you use the Vertex- or FragmentShader class." );

	// load in the shader and compile it
	glShaderSourceARB (shader, 1, (const GLcharARB**)&source, &length);
	glCompileShaderARB (shader);

	// see if the code was compiled successfully
   GLint compiled;
	glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if ( compiled != GL_TRUE )
   {
		GLint maxLength = 0;
		glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		GLcharARB *log = new GLcharARB[maxLength];
		glGetInfoLogARB(shader, maxLength, &length, log);
      Log::getInstance().info("Compiler log: %s", log);
		delete[] log;
		return false;
	}
	return true;
}

/// \fn Shader::load (const char* filename)
/// \brief Loads in a shader from a file.
/// \param[in] filename name of the shader file
/// \param     length   set to length of the code
/// \returns pointer to the source code loaded, user must release memory.
GLcharARB* Shader::load (const char* filename, int& length)
{
   std::ifstream file (filename, std::ios::binary );
	if ( !file.is_open() )
   {
      Log::getInstance().error("Shader.load: Can not open shader file: %s", filename);
		return false;
	}

	// determine file length
	file.seekg (0, std::ios::end);
	length = file.tellg();
	file.seekg (0, std::ios::beg);

	// read in the complete data
	GLcharARB* source = new GLcharARB[length+1];
	if ( source == NULL )
   {
      Log::getInstance().error("Shader.load: No memory available for source.");
		return false;
	}

	memset (source, 0, length+1);
	file.read (source, length);
	file.close ();
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
	if (shader != 0) {
		glDeleteObjectARB (shader);
		shader = 0;
	}
}

/*!
    \fn VertexShader::VertexShader()
	 \brief Creates a vertex shader object.
 */
VertexShader::VertexShader()
{
	handle( glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB) );
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
	handle( glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB) );
	if ( handle() == NULL )
   {
      Log::getInstance().error("FragmentShader: Can not create shader object.");
   }
}

/******************************************************
 * ShaderObject class
 */

/// \fn ShaderObject::ShaderObject()
/// \brief Constructor which is used to initialize member variables.
ShaderObject::ShaderObject():
   program(0)
{
}

/// \fn ShaderObject::~ShaderObject()
/// \brief Destructor which releases member variables.
ShaderObject::~ ShaderObject()
{
   release();
}

/*!
    \fn ShaderObject::create()
	 \brief Creates the OpenGL program object which is needed for the vertex- and fragment shaders.
	 \retval true the object is created successfully
	 \retval false the object could not be created
 */
bool ShaderObject::create()
{
	program = glCreateProgramObjectARB();
	if ( program == NULL )
   {
      Log::getInstance().error("ShaderObject.create: Could not create program object.");
		return false;
	}
	return true;
}

void ShaderObject::release ()
{
	// delete the program object
	glDeleteObjectARB (program);
	program = 0;
}

/*!
    \fn ShaderObject::link()
	 \brief This function does the actuall linking of the vertex- and fragment shaders added to this object. This should
	 be done before any of function of this class is called (with exception of the addShader function).
	 \retval true the shaders are successfully linked
	 \retval false the where no shaders added, or linking failed (log is generated)
 */
bool ShaderObject::link()
{
	GLint linked;
	GLenum glErr;

	if (shaders.size() == 0)
   {
		Log::getInstance().error("ShaderObject.link: there are no shaders to link.");
		return false;
	}
	glGetError();

	// add the shaders to the program
   std::vector<Shader*>::iterator it = shaders.begin();
	for ( ; it != shaders.end(); ++it )
   {
      Shader* pshader = *it;
		glAttachObjectARB (program, pshader->handle());
		pshader->release();
      delete pshader;
	}
	shaders.clear();

	// now link them & make sure it went ok
	glLinkProgramARB (program);

	glErr = glGetError();
   while (glErr != GL_NO_ERROR)
   {
	   Log::getInstance().error("GLerror: %s", (char*)gluErrorString(glErr));
      glErr = glGetError();
   }

	glGetObjectParameterivARB (program, GL_OBJECT_LINK_STATUS_ARB, (GLint*)&linked);
	if ( linked != GL_TRUE )
   {
		// display linker error message
		GLint length;
		glGetObjectParameterivARB (program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
		GLcharARB *log = new GLcharARB[length];
		glGetInfoLogARB (program, length, &length, log);
		Log::getInstance().error("Linker log: %s", log);
		delete[] log;
		return false;
	}
	return true;
}

/// \fn ShaderObject::valid() const
/// \brief Validates the shader object. If it is invalid a dump is added to the log file.
bool ShaderObject::valid() const
{
	int valid;
   glValidateProgramARB(program);
   glGetObjectParameterivARB(program, GL_OBJECT_VALIDATE_STATUS_ARB, (GLint*)&valid);

   if ( valid != GL_TRUE )
	{
      GLint length;

      // display linker error message
      glGetObjectParameterivARB(program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
      GLcharARB *log = new GLcharARB[length];
      glGetInfoLogARB( program, length, &length, log );
      Log::getInstance().error( "Validate log: %s", log );
      delete[] log;
   }

   return ( valid != GL_TRUE );
}

