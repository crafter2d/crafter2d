
#include "device.h"

#include "core/content/contentmanager.h"

#include "text/textrenderer.h"
#include "effect.h"
#include "rendercontext.h"

namespace Graphics
{

   Device::Device():
      mpContentManager(NULL),
      mpContext(NULL)
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
      mpContext = createRenderContext();
      return mpContext != NULL;
   }

   void Device::destroy()
   {
      mpContext->destroy();
      delete mpContext;
      mpContext = NULL;
   }

   Effect* Device::createEffect(const String& filename)
   {
      return mpContentManager->loadContent<Effect>(filename);
   }

};
