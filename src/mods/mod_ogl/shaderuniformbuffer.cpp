
#include "shaderuniformbuffer.h"

#include "GL/gl3w.h"

#include <memory>

#include "core/defines.h"

namespace Graphics
{

ShaderUniformBuffer::ShaderUniformBuffer(GLuint program, GLuint block):
   mProgram(program),
   mBlock(block),
   mBuffer(0),
   mBufferSize(0),
   mpElements(nullptr),
   mElementNr(0)
{
}

ShaderUniformBuffer::~ShaderUniformBuffer()
{
   delete[] mpElements;
   mElementNr = 0;
}

bool ShaderUniformBuffer::create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr)
{
   C2D_UNUSED(device);
   
   ASSERT(mProgram != GL_INVALID_INDEX);
   ASSERT(mBlock != GL_INVALID_INDEX);

   return createUniforms(pdescs, nr) && createBuffer();
}

void ShaderUniformBuffer::release()
{
   glBindBuffer(GL_UNIFORM_BUFFER, 0);
   glDeleteBuffers(1, &mBuffer);

   mBuffer = 0;
}

bool ShaderUniformBuffer::createBuffer()
{
   glGenBuffers(1, &mBuffer);
   glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
   glBufferData(GL_UNIFORM_BUFFER, mBufferSize, nullptr, GL_DYNAMIC_DRAW);
   glBindBufferBase(GL_UNIFORM_BUFFER, mBlock, mBuffer);

   return true;
}

bool ShaderUniformBuffer::createUniforms(UNIFORM_BUFFER_DESC* pdescs, int nr)
{
   glGetActiveUniformBlockiv(mProgram, mBlock, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &mElementNr);
   GLuint* pindices = new GLuint[mElementNr];
   glGetActiveUniformBlockiv(mProgram, mBlock, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*) pindices);

   GLint maxlength;
   glGetProgramiv(mProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxlength);
   auto pname = std::make_unique<GLchar[]>(maxlength);
   
   mpElements = new BLOCK_UNIFORM[mElementNr];
   for ( int index = 0; index < mElementNr; ++index )
   {
      BLOCK_UNIFORM& element = mpElements[index];

      int length;
      glGetActiveUniformName(mProgram, pindices[index], maxlength, &length, pname.get());
      element.name = String::fromUtf8(pname.get());

      glGetActiveUniformsiv(mProgram, 1, &pindices[index], GL_UNIFORM_OFFSET, &element.offset);
      
      int offset = 0;
      for ( int src = 0; src < nr; ++src )
      {
         UNIFORM_BUFFER_DESC& desc = pdescs[src];
         if ( desc.name == element.name )
         {
            element.source_offset = offset;
            element.size = desc.size;
            break;
         }
         else
         {
            offset += desc.size;
         }
      }
   }

   // determine total buffer size that needs to be allocated
   glGetActiveUniformBlockiv(mProgram, mBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &mBufferSize);

   return true;
}

void ShaderUniformBuffer::set(RenderContext& context, const void* pdata, const int size)
{
   C2D_UNUSED(context);
   glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
   glBufferSubData(GL_UNIFORM_BUFFER, 0, size, pdata);
}

void ShaderUniformBuffer::enable() const
{
   //glBindBufferRange(GL_UNIFORM_BUFFER, mBlock, mBuffer, 0, mBufferSize);
   glBindBufferBase(GL_UNIFORM_BUFFER, mBlock, mBuffer);
}

} // namespace Graphics
