
#ifndef SHADER_UNIFORM_BLOCK_H
#define SHADER_UNIFORM_BLOCK_H

#include <GL/glew.h>

#include "core/graphics/uniformbuffer.h"

namespace Graphics
{
   class ShaderUniformBuffer : public UniformBuffer
   {
   public:
      ShaderUniformBuffer(GLuint program, GLuint block);
      virtual ~ShaderUniformBuffer();

      virtual bool create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr) override;
      virtual void release() override;

      /// set the data into the uniform buffer
      virtual void set(RenderContext& context, const void* pdata, const int size) override;

      void enable() const;

   private:

      struct BLOCK_UNIFORM
      {
         String name;
         GLint  offset;
         GLint  size;
         int    source_offset;
      };

    // operations
      bool createBuffer();
      bool createUniforms(UNIFORM_BUFFER_DESC* pdescs, int nr);
     
    // data
      GLuint   mProgram;
      GLuint   mBlock;
      GLuint   mBuffer;
      GLint    mBufferSize;
      
      BLOCK_UNIFORM* mpElements;
      int            mElementNr;
   };
}

#endif // SHADER_UNIFORM_BLOCK_H
