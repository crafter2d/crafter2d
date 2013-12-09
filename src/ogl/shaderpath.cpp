
#include "shaderpath.h"

#include "core/graphics/texture.h"
#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/log/log.h"
#include "core/streams/datastream.h"
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
    \fn ShaderPath::load(VertexLayout* playout, DataStream& vertexshader, DataStream& pixelshader)
	\brief Loads in a vertex and fragment shader.

	This function loads the supplied vertex and fragment shaders into the shader
	object of this path. Both parameters are optional and maybe NULL. The shader
	files should contain valid GLSL code, otherwise an linking error message will
	be saved in the log file.

	\param vertex The vertex shader.
	\param fragment The fragment shader.
	\retval true the shader objects have been successfully linked.
	\retval false otherwise, consult the log file for compiler/linker specific errors.
 */
bool ShaderPath::create(VertexLayout* playout, DataStream& vertexshader, DataStream& pixelshader)
{
	Log& log = Log::getInstance ();
	shader.create();

	// try to load and add the vertex shader
   AutoPtr<VertexShader> vs = new VertexShader();
   if ( !vs->compile(vertexshader.getData(), vertexshader.getDataSize()) )
   {
      log.error("ShaderPath.load: Failed to compile vertex shader");
		return false;
	}
	
	// try to load and add the fragment shader
   AutoPtr<FragmentShader> fs = new FragmentShader();
   if ( !fs->compile(pixelshader.getData(), pixelshader.getDataSize()) )
   {
      log.error("ShaderPath.load: Failed to load or compile fragment shader");
      return false;
   }

   shader.addShader(vs.release());
   shader.addShader(fs.release());
	
   bool success = false;
   if ( shader.link() )
   {
      setupIndices(*playout);

      success = true;
   }

   return success;
}

void ShaderPath::release ()
{
	shader.release ();
}

void ShaderPath::enable(RenderContext& context) const
{
	shader.enable();
}

void ShaderPath::disable(RenderContext& context) const
{
	shader.disable();
}

UniformBuffer* ShaderPath::getUniformBuffer(const String& name) const
{
   return shader.getUniformBuffer(name);
}

bool ShaderPath::bindTexture(RenderContext& context, int stage, const Texture& texture)
{
   bool result = shader.bindTexture(stage, texture);
   texture.enable(context, stage);
   return result;
}

void ShaderPath::setupIndices(VertexLayout& layout)
{
   shader.enable();

   GLint program;
   glGetIntegerv(GL_CURRENT_PROGRAM, &program);

   for ( int index = 0; index < layout.getSize(); ++index )
   {
      VertexLayoutElement& field = layout[index];
      field.index = glGetAttribLocation(program, field.semantic.toUtf8().c_str());
   }

   shader.disable();
}
