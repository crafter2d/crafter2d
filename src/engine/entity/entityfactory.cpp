
#include "entityfactory.h"

#include "core/smartptr/autoptr.h"

#include "engine/components/componentfactory.h"
#include "engine/entity.h"

#include "entitydefinition.h"

EntityFactory::EntityFactory():
   mFactories(),
   mDefinitions(),
   mLoader()
{
}

void EntityFactory::registerComponentFactory(ComponentFactory* pfactory)
{
   mFactories.insert(std::make_pair(pfactory->getComponentType(), pfactory));

   mLoader.registerLoader(pfactory->createLoader());
}

Entity* EntityFactory::create(const String& filename)
{
   Entity* presult = NULL;

   EntityDefinition* pdefinition = findOrCreate(filename);
   if ( pdefinition != NULL )
   {
      presult = instantiate(*pdefinition);
      presult->setClassName(pdefinition->getClassName());
      presult->setFilename(filename);
      presult->initialize();
   }

   return presult;
}

EntityDefinition* EntityFactory::findOrCreate(const String& filename)
{
   EntityDefinition* presult = NULL;

   DefinitionMap::iterator it = mDefinitions.find(filename);
   if ( it != mDefinitions.end() )
   {
      presult = it->second;
   }
   else
   {
      presult = mLoader.load(filename);
      if ( presult != NULL )
      {
         mDefinitions.insert(std::make_pair(filename, presult));
      }
   }
   return presult;
}

Entity* EntityFactory::instantiate(const EntityDefinition& definition)
{
   const EntityDefinition::CompDefs& definitions = definition.getComponentDefinitions();

   AutoPtr<Entity> result = new Entity();

   for ( std::size_t index = 0; index < definitions.size(); ++index )
   {
      const ComponentDefinition& compdef = *definitions[index];

      ASSERT(mFactories.find(compdef.getComponentType()) != mFactories.end());
      ComponentFactory& factory = *mFactories[compdef.getComponentType()];

      Component* pcomponent = factory.instantiate(compdef);
      if ( pcomponent != NULL )
      {
         result->addComponent(pcomponent);
      }
   }

   return result.release();
}
