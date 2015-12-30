
#include "oglindexbuffer.h"

#include <GL/glew.h>

#include "core/defines.h"

using namespace Graphics;

OGLIndexBuffer::OGLIndexBuffer():
   IndexBuffer(),
   mNativeType(0),
   mLocked(false)
{
}

OGLIndexBuffer::~OGLIndexBuffer()
{
   release();
}

unsigned int OGLIndexBuffer::getNativeType() const
{
   return mNativeType;
}

bool OGLIndexBuffer::create(IndexFormat format, int elements, void* data)
{
   bool result = IndexBuffer::create(format, elements, data);
   if ( result )
   {
      int typesize = 0;

      switch ( format )
      {
      case eByte:
         mNativeType = GL_UNSIGNED_BYTE;
         typesize = sizeof(char);
         break;
      case eShort:
         mNativeType = GL_UNSIGNED_SHORT;
         typesize = sizeof(unsigned short);
         break;
      case eInt:
         mNativeType = GL_UNSIGNED_INT;
         typesize = sizeof(unsigned int);
         break;
      };

      unsigned int size = typesize * elements;

      glGenBuffers(1, &mBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
   }

   return result;
}

void OGLIndexBuffer::release()
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glDeleteBuffers(1, &mBuffer);
}

void OGLIndexBuffer::enable(RenderContext& context) const
{
   ASSERT(!mLocked);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
}

void OGLIndexBuffer::disable(RenderContext& context) const
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void* OGLIndexBuffer::lock(RenderContext& context)
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
	void* pointer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if ( pointer != NULL )
   {
		mLocked = true;
   }
	return pointer;
}

void OGLIndexBuffer::unlock(RenderContext& context)
{
   ASSERT(mLocked);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   mLocked = false;
}
