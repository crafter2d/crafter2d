
#include "indexbuffer.h"

#include "core/defines.h"

using namespace Graphics;

IndexBuffer::IndexBuffer():
   mFormat(eInt),
   mElements(0)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::create(IndexFormat format, int elements, void* pdata)
{
   C2D_UNUSED(pdata);
   
   mFormat = format;
   mElements = elements;

   return true;
}

void IndexBuffer::release()
{
}
