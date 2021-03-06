
#include "shaderpath.h"

#include <memory>

#include "core/graphics/texture.h"
#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/string/string.h"

#include "shaderuniformbuffer.h"

using namespace Graphics;

ShaderPath::ShaderPath():
   CodePath(),
   mShader()
{
}

/*!
    \fn ShaderPath::load(VertexLayout* playout, DataStream& vertexshader, DataStream& pixelshader)
	\brief Loads in a vertex and fragment shader.

	This function loads the supplied vertex and fragment shaders into the shader
	object of this path. Both parameters are optional and maybe nullptr. The shader
	files should contain valid GLSL code, otherwise an linking error message will
	be saved in the log file.

	\param vertex The vertex shader.
	\param fragment The fragment shader.
	\retval true the shader objects have been successfully linked.
	\retval false otherwise, consult the log file for compiler/linker specific errors.
 */
bool ShaderPath::create(VertexLayout* playout, DataStream& vertexshader, DataStream& geometryshader, DataStream& pixelshader)
{
	Log& log = Log::getInstance ();
	mShader.create();

   setVertexLayout(playout);

	// try to load and add the vertex shader
   VertexShader vs;
   if ( !vs.compile(vertexshader.getData(), vertexshader.getDataSize()) )
   {
      log.error("ShaderPath.load: Failed to compile vertex shader");
		return false;
	}

   // try to load the geometry shader
   if ( geometryshader.getDataSize() > 0 )
   {
      GeometryShader gs;
      if ( !gs.compile(geometryshader.getData(), geometryshader.getDataSize()) )
      {
         log.error("ShaderPath.load: Failed to compile geometry shader");
         return false;
      }

      mShader.addShader(std::move(gs));
   }
	
	// try to load and add the fragment shader
   FragmentShader fs;
   if ( !fs.compile(pixelshader.getData(), pixelshader.getDataSize()) )
   {
      log.error("ShaderPath.load: Failed to load or compile fragment shader");
      return false;
   }

   mShader.addShader(std::move(vs));
   mShader.addShader(std::move(fs));
	
   return mShader.link(*playout);
}

void ShaderPath::release()
{
	mShader.release();
}

void ShaderPath::enable(RenderContext& context) const
{
   C2D_UNUSED(context);
	mShader.enable();
}

void ShaderPath::disable(RenderContext& context) const
{
   C2D_UNUSED(context);
	mShader.disable();
}

UniformBuffer* ShaderPath::getUniformBuffer(const String& name) const
{
   return mShader.getUniformBuffer(name);
}

void ShaderPath::bindTexture(RenderContext& context, int stage, const Texture& texture)
{
   mShader.bindTexture(stage, texture);
   texture.enable(context, stage);
}

void ShaderPath::setConstantBuffer(RenderContext& context, int slot, const UniformBuffer& buffer)
{
   C2D_UNUSED(context);
   static_cast<const ShaderUniformBuffer&>(buffer).enable();
}
