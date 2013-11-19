
#ifndef VERTEX_LAYOUT_ELEMENT_H
#define VERTEX_LAYOUT_ELEMENT_H

#include "core/string/string.h"

namespace Graphics
{
   struct VertexLayoutElement
   {
      String semantic;
      int index;
	   int size;
	   int pos;
   };
}

#endif // VERTEX_LAYOUT_ELEMENT_H
