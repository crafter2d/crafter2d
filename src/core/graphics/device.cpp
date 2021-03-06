
#include "device.h"

#include "core/content/contentmanager.h"

#include "text/textrenderer.h"
#include "effect.h"
#include "rendercontext.h"

namespace Graphics
{

   Device::Device():
      mpContentManager(nullptr),
      mpContext(nullptr)
   {
   }

   Device::~Device()
   {
   }

   // - Get/set

   c2d::ContentManager& Device::getContentManager()
   {
      ASSERT_PTR(mpContentManager);
      return *mpContentManager;
   }

   void Device::setContentManager(c2d::ContentManager& contentmanager)
   {
      mpContentManager = &contentmanager;
      mpContentManager->setDevice(*this);
   }

   RenderContext& Device::getContext()
   {
      ASSERT_PTR(mpContext);
      return *mpContext;
   }

   Font& Device::getFont(const String& name)
   {
      TextRenderer& renderer = mpContext->getTextRenderer();
      return renderer.getFont(name);
   }

   // - Creation

   bool Device::create(GameWindow& window)
   {
      C2D_UNUSED(window);
      
      mpContext = createRenderContext();
      return mpContext != nullptr;
   }

   void Device::destroy()
   {
      mpContext->destroy();
      delete mpContext;
      mpContext = nullptr;
   }

   Effect* Device::createEffect(const String& filename)
   {
      return mpContentManager->loadContent<Effect>(filename);
   }

};
