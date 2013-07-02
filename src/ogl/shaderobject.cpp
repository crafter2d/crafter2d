
#include "shaderobject.h"
#ifndef JENGINE_INLINE
#  include "shaderobject.inl"
#endif

#include <GL/GLee.h>
#include <GL/glu.h>

#include "core/log/log.h"
#include "core/string/string.h"

using namespace Graphics;

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

/// \fn ShaderObject::getAttribute(const char* name)
/// \brief Try to find a custom attribute in the GLSL shader by name.
/// \return The index of the attribute that is currently linked to it or -1 if the attribute was not found.
GLint ShaderObject::getAttribute(const char* name) const
{
	return glGetAttribLocationARB (program, name);
}

GLint ShaderObject::getUniformLocation(const String& name) const
{
   const std::string loc = name.toUtf8();
   return glGetUniformLocationARB(program, loc.c_str());
}
