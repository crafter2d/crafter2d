
#include "device.h"

#include "core/content/contentmanager.h"

#include "effect.h"

namespace Graphics
{

   Device::Device(ContentManager& contentmanager):
      mContentManager(contentmanager)
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

   // - Creation

   Effect* Device::createEffect(const String& filename)
   {
      return mContentManager.loadContent<Effect>(filename);
   }

};
