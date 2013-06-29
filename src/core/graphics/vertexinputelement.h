
#ifndef VERTEX_INPUT_ELEMENT_H
#define VERTEX_INPUT_ELEMENT_H

#include "core/string/string.h"

namespace Graphics
{
   struct VertexInputElement
   {
      String semantic;
      int index;
	   int size;
	   int pos;
	   int flags;
   };
}

#endif // VERTEX_INPUT_ELEMENT_H
