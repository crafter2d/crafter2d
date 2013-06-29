
#ifndef VERTEX_INPUT_LAYOUT_H
#define VERTEX_INPUT_LAYOUT_H

#include "core/core_base.h"

namespace Graphics
{
   const int INPUT_XY   = 1;
   const int INPUT_XYZ  = 2;
   const int INPUT_XYZW = 4;
   const int INPUT_Normal = 8;
   const int INPUT_Diffuse = 16;
   const int INPUT_Specular = 32;
   const int INPUT_Tex0 = 64;
   const int INPUT_Tex1 = 128;
   const int INPUT_Tex2 = 256;
   const int INPUT_Tex3 = 512;
   const int INPUT_Tex4 = 1024;
   const int INPUT_Tangent = 2048;
   const int INPUT_Binormal = 4096;

   struct VertexInputElement;

   class CORE_API VertexInputLayout
   {
   public:
      VertexInputLayout(int fvf);
      ~VertexInputLayout();

      const VertexInputElement& operator[](int index) const;

    // get/set
      int getSize() const;
      int getStride() const;

   private:

    // statics
      static int calculateFieldCount(int fvf);
      static int calculateStride(int fvf);

    // operations
      void createElements(int fvf);

      VertexInputElement* mpElements;
      int                 mCount;
      int                 mStride;

   };
}

#endif // VERTEX_INPUT_LAYOUT_H
