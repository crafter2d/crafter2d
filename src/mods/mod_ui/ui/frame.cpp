
#include "frame.h"

#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/effect.h"
#include "core/graphics/utils.h"
#include "core/graphics/rendertarget.h"
#include "core/graphics/geometrybuffer.h"
#include "core/string/string.h"

#include "graphics2d.h"
#include "system.h"

using namespace Graphics;

namespace ui
{
   Frame::Frame(Window* pparent) :
      Window(pparent),
      mpTarget(NULL)
   {
   }

   // - Operations

   bool Frame::create()
   {
      Device& device = System::getInstance().getDevice();

      mpTarget = device.createRenderTarget();
      mpBuffer = GeometryBuffer<UIVertex>::createTriangleBuffer(device, UTEXT("shaders/basic"), 512);
      if ( mpTarget == NULL || mpBuffer == NULL )
      {
         return false;
      }

      return true;
   }

   /*
   void Frame::draw()
   {
      RenderContext& context = System::getInstance().getContext();
      context.setRenderTarget(*mpTarget);

      Graphics2D graphics;
      graphics.setGeometryBuffer(*mpBuffer);
      draw(graphics);

      mpBuffer->draw(context);
   }
   */
}
