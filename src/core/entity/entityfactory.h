
#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <map>

class ComponentFactory;
class Entity;
class EntityDefinition;
class String;

class EntityFactory
{
public:
   EntityFactory();

   void registerComponentFactory(ComponentFactory* pfactory);

   Entity* create(const String& filename);

private:
   typedef std::map<int, ComponentFactory*> Factories;
   typedef std::map<String, EntityDefinition*> DefinitionMap;

 // operations
   EntityDefinition* findOrCreate(const String& filename);
   Entity* instantiate(const EntityDefinition& definition);

 // data
   Factories      mFactories;
   DefinitionMap  mDefinitions;
};

#endif // ENTITY_FACTORY_H
