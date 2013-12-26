
#include "device.h"

#include "core/content/contentmanager.h"

#include "effect.h"

namespace Graphics
{

   Device::Device(ContentManager& contentmanager):
      mContentManager(contentmanager),
      mpContext(NULL)
   {
   }

   Device::~Device()
   {
   }

   // - Get/set

   ContentManager& Device::getContentManager()
   {
      return mContentManager;
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
      return mContentManager.loadContent<Effect>(filename);
   }

};
