
#include "modulecollection.h"

#include "core/system/exception.h"

namespace c2d
{
   ModuleCollection::ModuleCollection() :
      mModules()
   {
   }

   ModuleCollection::ModuleCollection(ModuleCollection&& that):
      mModules(std::move(that.mModules))
   {
   }

   ModuleCollection::~ModuleCollection()
   {
      mModules.clear();
   }

   // - Operators

   Module* ModuleCollection::operator[](const Uuid& uuid)
   {
      auto it = mModules.find(uuid);
      return it != mModules.end() ? it->second : nullptr;
   }

   ModuleCollection& ModuleCollection::operator=(ModuleCollection&& that)
   {
      std::swap(mModules, that.mModules);
      return *this;
   }

   ModuleCollection& ModuleCollection::operator=(const ModuleCollection& that)
   {
      mModules.clear();
      add(that);
      return *this;
   }
   
   // - Query

   int ModuleCollection::size() const
   {
      return mModules.size();
   }

   // - Maintenance

   void ModuleCollection::add(Module* pmodule)
   {
      auto it = mModules.find(pmodule->getUuid());
      if ( it != mModules.end() && it->second != pmodule )
      {
         throw new c2d::Exception(UTEXT("Can not add module with duplicate key."));
      }

      mModules.insert(std::make_pair(pmodule->getUuid(), pmodule));
   }

   void ModuleCollection::add(const ModuleCollection& that)
   {
      for ( auto& pair : mModules )
      {
         Module* pmodule = pair.second;
         add(pmodule);
      }
   }

   void ModuleCollection::clear()
   {
      mModules.clear();
   }

   // - Filtering

   ModuleCollection ModuleCollection::filter(ModuleKind kind)
   {
      ModuleCollection result;
      for ( auto& pair : mModules )
      {
         Module* pmodule = pair.second;
         if ( pmodule->getKind() == kind )
         {
            result.add(pmodule);
         }
      }
      return result;
   }
}
