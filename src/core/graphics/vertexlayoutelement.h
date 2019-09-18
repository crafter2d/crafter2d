
#ifndef VERTEX_LAYOUT_ELEMENT_H
#define VERTEX_LAYOUT_ELEMENT_H

#include "core/string/string.h"

namespace Graphics
{
   struct VertexLayoutElement
   {
      VertexLayoutElement():
         semantic(),
         pos(0),
         type(0),
         index(0)
      {
      }

      VertexLayoutElement(const String& _semantic, uint32_t _pos, uint32_t _type) :
         semantic(_semantic),
         pos(_pos),
         type(_type),
         index(0)
      {
      }

      String semantic;
	   uint32_t pos;
      uint32_t type;
      int index;
   };
}

#endif // VERTEX_LAYOUT_ELEMENT_H
