
#include "modulecollection.h"

ModuleCollection::ModuleCollection(Module::Type filter):
   mModules(),
   mFilterType(filter)
{
}

ModuleCollection::~ModuleCollection()
{
   mModules.clear();
}

// - Operators

Module& ModuleCollection::operator[](int index)
{
   return* mModules[index];
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
   if ( mFilterType == Module::eInvalid || pmodule->getType() == mFilterType )
   {
      mModules.push_back(pmodule);
   }
}

void ModuleCollection::add(const ModuleCollection& that)
{
   for ( std::size_t index = 0; index < that.mModules.size(); ++index )
   {
      add(that.mModules[index]);
   }
}

void ModuleCollection::clear()
{
   mModules.clear();
}
