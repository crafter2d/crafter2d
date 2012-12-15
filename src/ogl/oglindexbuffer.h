
#ifndef OPENGL_INDEX_BUFFER_H
#define OPENGL_INDEX_BUFFER_H

#include "core/graphics/indexbuffer.h"

namespace Graphics
{
   class OGLIndexBuffer : public IndexBuffer
   {
   public:
      OGLIndexBuffer();

    // query
      unsigned int getNativeType() const;

      virtual bool create(IndexFormat format, int elements, void* data);
      virtual void release();

      virtual void enable() const;
      virtual void disable() const;

      virtual void* lock();
      virtual void unlock();

   private:
      unsigned int mBuffer;
      unsigned int mNativeType;
      bool         mLocked;
   };
};

#endif // OPENGL_INDEX_BUFFER_H
