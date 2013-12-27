
#include "device.h"

#include "core/content/contentmanager.h"

#include "effect.h"

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

   ContentManager& Device::getContentManager()
   {
      ASSERT_PTR(mpContentManager);
      return *mpContentManager;
   }

   void Device::setContentManager(ContentManager& contentmanager)
   {
      mpContentManager = &contentmanager;
   }

   RenderContext& Device::getContext()
   {
      ASSERT_PTR(mpContext);
      return *mpContext;
   }

   // - Creation

   bool Device::create(int windowhandle, int width, int height)
   {
      mpContext = createRenderContext();
      return mpContext != NULL;
   }

   Effect* Device::createEffect(const String& filename)
   {
      return mpContentManager->loadContent<Effect>(filename);
   }

};
