
#include "shaderobject.h"
#ifndef JENGINE_INLINE
#  include "shaderobject.inl"
#endif

#include <GL/glew.h>
#include <GL/glu.h>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/graphics/texture.h"
#include "core/log/log.h"
#include "core/string/string.h"

#include "shaderuniformbuffer.h"

namespace Graphics
{

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
	program = glCreateProgram();
	if ( program == 0 )
   {
      Log::getInstance().error("ShaderObject.create: Could not create program object.");
		return false;
	}
	return true;
}

void ShaderObject::release ()
{
	// delete the program object
	glDeleteProgram(program);
	program = 0;
}

/*!
    \fn ShaderObject::link()
	 \brief This function does the actuall linking of the vertex- and fragment shaders added to this object. This should
	 be done before any of function of this class is called (with exception of the addShader function).
	 \retval true the shaders are successfully linked
	 \retval false the where no shaders added, or linking failed (log is generated)
 */
bool ShaderObject::link(VertexLayout& layout)
{
	GLint linked;
	GLenum glErr;

	if ( shaders.empty() )
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
		glAttachShader(program, pshader->handle());
		pshader->release();
      delete pshader;
	}
	shaders.clear();

	// now link them & make sure it went ok
	glLinkProgram(program);

	glErr = glGetError();
   while (glErr != GL_NO_ERROR)
   {
	   Log::getInstance().error("GLerror: %s", (char*)gluErrorString(glErr));
      glErr = glGetError();
   }

   glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if ( linked != GL_TRUE )
   {
		// display linker error message
		GLint length;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar *log = new GLchar[length];
		glGetProgramInfoLog (program, length, &length, log);
		Log::getInstance().error("Linker log: %s", log);
		delete[] log;
		return false;
	}

   linkInput(layout);

	return true;
}

void ShaderObject::linkInput(VertexLayout& layout)
{
   glUseProgram(program);
   for ( int index = 0; index < layout.getSize(); ++index )
   {
      VertexLayoutElement& field = layout[index];
      // TODO: index is missing!
      // field.index = glGetAttribLocation(program, field.semantic.toUtf8().c_str());
      GLenum err = glGetError();
      if ( err != GL_NO_ERROR )
      {
         String oglerr(std::string((const char*)gluErrorString(err)));
         String msg(UTEXT("Could not bind attribute '{0}' for shader: {1}").arg(0, field.semantic).arg(1, oglerr));
         Log::getInstance().error(msg.toUtf8().c_str());
      }
   }
   glUseProgram(0);
}

/// \fn ShaderObject::valid() const
/// \brief Validates the shader object. If it is invalid a dump is added to the log file.
bool ShaderObject::valid() const
{
	int valid;
   glValidateProgram(program);
   glGetProgramiv(program, GL_VALIDATE_STATUS, (GLint*)&valid);

   if ( valid != GL_TRUE )
	{
      GLint length;

      // display linker error message
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
      GLchar *log = new GLcharARB[length];
      glGetProgramInfoLog( program, length, &length, log );
      Log::getInstance().error( "Validate log: %s", log );
      delete[] log;
   }

   return ( valid != GL_TRUE );
}

ShaderUniformBuffer* ShaderObject::getUniformBuffer(const String& name) const
{
   ShaderUniformBuffer* presult = nullptr;
   std::string uname = name.toUtf8();
   GLuint index = glGetUniformBlockIndex(program, uname.c_str());
   if ( index != GL_INVALID_INDEX )
   {
      presult = new ShaderUniformBuffer(program, index);
   }

   return presult;
}

bool ShaderObject::bindTexture(int stage, const Texture& texture)
{
   C2D_UNUSED(texture);
   
   std::string uni = "diffuseMap"; // texture.getUniform().toUtf8();
   GLint texloc = glGetUniformLocation(program, uni.c_str());
   glProgramUniform1i(program, texloc, stage);
   return glGetError() == GL_NO_ERROR;
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

} // namespace Graphics
