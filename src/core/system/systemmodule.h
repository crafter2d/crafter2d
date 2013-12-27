
#ifndef SYSTEM_MODULE_H
#define SYSTEM_MODULE_H

#include "core/modules/module.h"
#include "core/core_base.h"

namespace c2d
{
   class System;

   class CORE_API SystemModule : public Module
   {
   public:
      SystemModule();

    // get/set
      System& getSystem();
      void    setSystem(System* psystem);

   private:

    // data
      System* mpSystem;
   };
}

#endif // SYSTEM_MODULE_H
