
#include "shaderpath.h"

#include "core/log/log.h"
#include "core/string/string.h"
#include "core/smartptr/autoptr.h"

#include "shaderuniformbuffer.h"

using namespace Graphics;

ShaderPath::ShaderPath():
   CodePath(),
   shader()
{
}

/*!
    \fn ShaderPath::load (VertexInputLayout& layout, const char* vertex, const char* fragment)
	\brief Loads in a vertex and fragment shader.

	This function loads the supplied vertex and fragment shaders into the shader
	object of this path. Both parameters are optional and maybe NULL. The shader
	files should contain valid GLSL code, otherwise an linking error message will
	be saved in the log file.

	\param vertex File name of the vertex shader (may be NULL).
	\param fragment File name of the fragment shader (may be NULL).
	\retval true the shader objects have been successfully linked.
	\retval false otherwise, consult the log file for compiler/linker specific errors.
 */
bool ShaderPath::load(VertexInputLayout& layout, const String& vertex, const String& fragment)
{
	Log& log = Log::getInstance ();
	shader.create();

	// try to load and add the vertex shader
   if ( !vertex.isEmpty() )
   {
		AutoPtr<VertexShader> vs = new VertexShader();
      if ( !vs->compile(vertex) )
      {
         log.error("GLSLPath.load: Failed to load or compile vertex shader '%s'", vertex.toUtf8().c_str());
			return false;
		}

      shader.addShader(vs.release());
	}

	// try to load and add the fragment shader
   if ( !fragment.isEmpty() )
   {
		AutoPtr<FragmentShader> fs = new FragmentShader();
      if ( !fs->compile(fragment) )
      {
         log.error("GLSLPath.load: Failed to load or compile fragment shader '%s'", fragment.toUtf8().c_str());
			return false;
		}

      shader.addShader(fs.release());
	}

	// link the shader
	return shader.link();
}

void ShaderPath::release ()
{
	shader.release ();
}

void ShaderPath::enable () const
{
	shader.enable();
}

void ShaderPath::disable () const
{
	shader.disable();
}

UniformBuffer* ShaderPath::getUniformBuffer(const String& name) const
{
   return shader.getUniformBuffer(name);
}

bool ShaderPath::bindTexture(const Texture& texture)
{
   return shader.bindTexture(texture);
}
