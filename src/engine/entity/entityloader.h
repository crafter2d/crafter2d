
#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include <map>

#include "core/string/string.h"

#include "engine/components/componentloader.h"

class EntityDefinition;

class EntityLoader
{
public:
   EntityLoader();

 // operations
   void registerLoader(ComponentLoader* ploader);

   EntityDefinition* load(const String& filename);

private:
   typedef std::map<String, ComponentLoader*> Loaders;

 // data
   Loaders mLoaders;
};

#endif // ENTITY_LOADER_H
