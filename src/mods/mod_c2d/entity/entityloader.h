
#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include <map>

#include "core/string/string.h"

class ComponentLoader;
class ChildDefinitionProto;
class EntityDefinitionProto;
class LinkDefinitionProto;
class TiXmlElement;

class EntityLoader
{
public:
   EntityLoader();

 // loading
   EntityDefinitionProto* load(const String& filename);

private:
   typedef std::map<String, ComponentLoader*> Loaders;
   
 // loading
   EntityDefinitionProto*  loadDefinition(const TiXmlElement& entity);

   void loadChildDefinition(EntityDefinitionProto& entity, const TiXmlElement& childelement);
   void loadLinkDefinition(EntityDefinitionProto& entity, const TiXmlElement& xmllink);

 // registration
   void registerLoader(ComponentLoader* ploader);
    
 // data
   Loaders mLoaders;
};

#endif // ENTITY_LOADER_H
