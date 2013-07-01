
#include "shaderpath.h"

#include "core/log/log.h"
#include "core/string/string.h"
#include "core/smartptr/autoptr.h"

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
         log.error("GLSLPath.load: Failed to load or compile vertex shader '%s'", vertex.getBuffer());
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
         log.error("GLSLPath.load: Failed to load or compile fragment shader '%s'", fragment.getBuffer());
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

CodePath::PathType ShaderPath::getType() const
{
   return CodePath::EGLSL;
}

int ShaderPath::getUniformLocation (const String& name) const
{
	return shader.getUniformLocation(name);
}

void ShaderPath::setUniformStateMatrix(int index) const
{
   shader.setUniformStateMatrix(index);
}

void ShaderPath::setUniform1i (int index, int val)
{
	shader.setUniform1i(index, val);
}

void ShaderPath::setUniform3f (int index, float x, float y, float z)
{
	shader.setUniform3f (index, x, y, z);
}

void ShaderPath::setUniform4f (int index, float x, float y, float z, float w)
{
	shader.setUniform4f (index, x, y, z, w);
}
