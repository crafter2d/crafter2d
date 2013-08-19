
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

ShaderUniformBuffer::~ShaderUniformBuffer()
{
   delete mpElements;
   mElementNr = 0;
}

bool ShaderUniformBuffer::create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr)
{
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

   mpElements = new BLOCK_UNIFORM[mElementNr];
   for ( int index = 0; index < mElementNr; ++index )
   {
      BLOCK_UNIFORM& element = mpElements[index];

      int length;
      glGetActiveUniformName(mProgram, pindices[index], maxlength, &length, pname);
      element.name = String::fromUtf8(pname);

      glGetActiveUniformsiv(mProgram, 1, &pindices[index], GL_UNIFORM_OFFSET, &element.offset);
      glGetActiveUniformsiv(mProgram, 1, &pindices[index], GL_UNIFORM_SIZE  , &element.size);

      int offset = 0;
      for ( int src = 0; src < nr; ++src )
      {
         UNIFORM_BUFFER_DESC& desc = pdescs[src];
         if ( desc.name == element.name )
         {
            element.source_offset = offset;
            break;
         }
         else
         {
            offset += desc.size;
         }
      }
   }

   // determine total buffer size that needs to be allocated
   mBufferSize = 0;
   for ( int index = 0; index < nr; ++index )
   {
      UNIFORM_BUFFER_DESC& desc = pdescs[index];
      mBufferSize += desc.size;
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
