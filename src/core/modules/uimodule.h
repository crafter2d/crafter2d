
#ifndef UI_MODULE_H
#define UI_MODULE_H

#include <memory>

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
         return *mSystem;
      }

   private:

      std::unique_ptr<UiSystem> mSystem;
   };
}

#endif // UI_MODULE_H
