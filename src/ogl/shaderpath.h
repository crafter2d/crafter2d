
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

	   virtual bool     load(VertexInputLayout& layout, const String& vertex, const String& fragment);
	   virtual void     release();

	   virtual void     enable() const;
	   virtual void     disable() const;

      virtual PathType getType() const;
	   virtual int      getUniformLocation(const String& name) const;

      virtual void     setUniformStateMatrix(int index) const;

	   virtual void     setUniform1i(int index, int val);
	   virtual void     setUniform3f(int index, float x, float y, float z);
	   virtual void     setUniform4f(int index, float x, float y, float z, float w);

   private:
	   ShaderObject shader;
   };
};

#endif // SHADER_PATH_H
