
#include "module.h"

#include "core/defines.h"

#include "modulemanager.h"

namespace c2d
{
   Module::Module(ModuleKind kind, const Uuid& uuid) :
      mpManager(nullptr),
      mKind(kind),
      mUuid(uuid)
   {
   }

   Module::~Module()
   {
   }

   // - Get/set

   ModuleManager& Module::getModuleManager()
   {
      ASSERT_PTR(mpManager);
      return *mpManager;
   }

   void Module::setModuleManager(ModuleManager& manager)
   {
      mpManager = &manager;
   }

   ModuleKind Module::getKind() const
   {
      return mKind;
   }

   const Uuid& Module::getUuid() const
   {
      return mUuid;
   }

   // - Operations

   void Module::initialize()
   {

   }

   Module& Module::lookupModule(const Uuid& uuid)
   {
      ASSERT_PTR(mpManager);
      Module* pmodule = mpManager->lookup(uuid);
      if ( pmodule == nullptr )
      {
         throw new std::exception();
      }
      return *pmodule;
   }
}
