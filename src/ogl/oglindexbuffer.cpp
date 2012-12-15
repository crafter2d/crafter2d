
#include "oglindexbuffer.h"

#include <GL/GLee.h>

using namespace Graphics;

OGLIndexBuffer::OGLIndexBuffer():
   IndexBuffer(),
   mNativeType(0)
{
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
   disable();

   glDeleteBuffers(1, &mBuffer);
}

void OGLIndexBuffer::enable() const
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
}

void OGLIndexBuffer::disable() const
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void* OGLIndexBuffer::lock()
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
	void* pointer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if ( pointer != NULL )
   {
		mLocked = true;
   }
	return pointer;
}

void OGLIndexBuffer::unlock()
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
   glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
   mLocked = false;
}
