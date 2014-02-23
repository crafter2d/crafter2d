
#ifndef __ScriptModule_h__
#define __ScriptModule_h__

#include "core/core_base.h"

#include "module.h"

namespace c2d
{
   class ScriptManager;

   class CORE_API ScriptModule : public Module
   {
   public:
      ScriptModule(ScriptManager* pmanager);

    // query
      ScriptManager& getManager();

   private:

    // data
      ScriptManager* mpManager;
   };
}

#endif // __ScriptModule_h__
