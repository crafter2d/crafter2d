
#ifndef SHADER_PATH_H
#define SHADER_PATH_H

#include "core/graphics/codepath.h"

#include "shaderobject.h"

namespace Graphics
{
   /*!
   @author Jeroen Broekhuizen
   \brief Code path that uses the shader objects as shader.

   This code path uses the GLSL shader objects for shading the worlds. This path is used by default both
   vertex and fragment shaders are supported on the target hardware.
    */
   class ShaderPath: public CodePath
   {
   public:
      ShaderPath();

    // query
      virtual UniformBuffer* getUniformBuffer(const String& name) const override;

    // operations
	   virtual bool     load(const VertexInputLayout& layout, const String& vertex, const String& fragment) override;
	   virtual void     release() override;

      virtual void     enable(RenderContext& context) const override;
	   virtual void     disable(RenderContext& context) const override;

      virtual bool     bindTexture(RenderContext& context, const Texture& uniform) override;

   private:
	   ShaderObject shader;
   };
};

#endif // SHADER_PATH_H
