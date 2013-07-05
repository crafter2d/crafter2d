
#include "shaderuniformbuffer.h"

#include "core/defines.h"

namespace Graphics
{

ShaderUniformBuffer::ShaderUniformBuffer(GLuint program, GLuint block):
   mProgram(program),
   mBlock(block),
   mBuffer(0),
   mpElements(NULL),
   mElementNr(NULL)
{
}

bool ShaderUniformBuffer::create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr)
{
   ASSERT(mProgram > 0);
   ASSERT(mBlock > 0);

   return createUniforms(pdescs, nr) && createBuffer();
}

bool ShaderUniformBuffer::createBuffer()
{
   glGetActiveUniformBlockiv(mProgram, mBuffer, GL_UNIFORM_BLOCK_DATA_SIZE, &mBufferSize);

   glGenBuffers(1, &mBuffer);
   glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
   glBufferData(GL_UNIFORM_BUFFER, mBufferSize, NULL, GL_DYNAMIC_DRAW);
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
   GLchar* pname = new GLchar[maxlength];

   BLOCK_UNIFORM* pelements = new BLOCK_UNIFORM[mElementNr];
   for ( int index = 0; index < mElementNr; ++index )
   {
      BLOCK_UNIFORM& element = pelements[index];

      int length;
      glGetActiveUniformName(mProgram, pindices[index], maxlength, &length, pname);
      element.name = String::fromUtf8(pname);

      glGetActiveUniformsiv(mProgram, 1, &pindices[index], GL_UNIFORM_OFFSET, &element.offset);
      glGetActiveUniformsiv(mProgram, 1, &pindices[index], GL_UNIFORM_SIZE  , &element.size);

      for ( int src = 0; src < nr; ++src )
      {
         UNIFORM_BUFFER_DESC& desc = pdescs[src];
         if ( desc.name == element.name )
         {
            element.source_offset = desc.offset;
            break;
         }
      }
   }

   return true;
}

void ShaderUniformBuffer::set(RenderContext& context, const void* pdata)
{
   glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
   glBufferData(GL_UNIFORM_BUFFER, mBufferSize, pdata, GL_DYNAMIC_DRAW);
}

void ShaderUniformBuffer::enable(RenderContext& context) const
{
   glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
}

} // namespace Graphics
