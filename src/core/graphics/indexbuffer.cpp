
#include "indexbuffer.h"

using namespace Graphics;

IndexBuffer::IndexBuffer()
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
