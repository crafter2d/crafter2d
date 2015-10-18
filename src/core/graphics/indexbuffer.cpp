
#include "indexbuffer.h"

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
   mFormat = format;
   mElements = elements;

   return true;
}

void IndexBuffer::release()
{
}
