
#include "modulecollection.h"

#include "core/system/exception.h"

namespace c2d
{
   ModuleCollection::ModuleCollection() :
      mModules()
   {
   }

   ModuleCollection::~ModuleCollection()
   {
      mModules.clear();
   }

   // - Operators

   Module* ModuleCollection::operator[](const Uuid& uuid)
   {
      ModuleMap::iterator it = mModules.find(uuid);
      return it != mModules.end() ? it->second : NULL;
   }

   ModuleCollection& ModuleCollection::operator=(const ModuleCollection& that)
   {
      mModules.clear();
      add(that);
      return *this;
   }

   ModuleCollectionIterator ModuleCollection::getIterator()
   {
      return ModuleCollectionIterator(mModules.begin(), mModules.end());
   }

   // - Query

   int ModuleCollection::size() const
   {
      return mModules.size();
   }

   // - Maintenance

   void ModuleCollection::add(Module* pmodule)
   {
      ModuleMap::iterator it = mModules.find(pmodule->getUuid());
      if ( it != mModules.end() && it->second != pmodule )
      {
         throw new c2d::Exception(UTEXT("Can not add module with duplicate key."));
      }

      mModules.insert(std::make_pair(pmodule->getUuid(), pmodule));
   }

   void ModuleCollection::add(const ModuleCollection& that)
   {
      for ( ModuleMap::const_iterator it = that.mModules.begin(); it != that.mModules.end(); ++it )
      {
         Module* pmodule = it->second;
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
      for ( ModuleMap::iterator it = mModules.begin(); it != mModules.end(); ++it )
      {
         Module* pmodule = it->second;
         if ( pmodule->getKind() == kind )
         {
            result.add(pmodule);
         }
      }
      return result;
   }
}
