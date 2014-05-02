
#ifndef __Frame_h__
#define __Frame_h__

#include "window.h"
#include "graphics2d.h"

namespace Graphics
{
   class RenderTarget;
   template <class T> class GeometryBuffer;
}

namespace ui
{
   class Frame : public Window
   {
   public:
      Frame(Window* pparent);

    // operations
      virtual bool create();
      
   private:

    // data
      Graphics::RenderTarget*             mpTarget;
      Graphics::GeometryBuffer<UIVertex>* mpBuffer;
   };
}

#endif // __Frame_h__
