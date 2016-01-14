
#ifndef UI_MODULE_H
#define UI_MODULE_H

#include "module.h"

namespace c2d
{
   class UiSystem;

   class CORE_API UiModule : public Module
   {
   public:
      explicit UiModule(UiSystem* psystem);
      virtual ~UiModule();

      // get/set
      UiSystem& getSystem() {
         return *mpSystem;
      }

   private:

      UiSystem* mpSystem;
   };
}

#endif // UI_MODULE_H
