
#include "entityfactory.h"

#include "core/smartptr/autoptr.h"

#include "engine/components/componentfactory.h"
#include "engine/components/querybodycomponentmessage.h"
#include "engine/physics/revolutejointdefinition.h"
#include "engine/entity.h"

#include "entitydefinition.h"
#include "entitylinkdefinition.h"

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
      presult->setFilename(filename);
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
   const EntityDefinition::Components& definitions = definition.getComponentDefinitions();

   AutoPtr<Entity> result = new Entity();

   const EntityDefinition::Children& children = definition.getChildren();
   for ( std::size_t index = 0; index < children.size(); ++index )
   {
      const EntityDefinition* pdef = children[index];
      Entity* pchild = instantiate(*pdef);
      pchild->setOffset(pdef->getOffset());
      result->add(pchild);
   }

   const EntityDefinition::Links& links = definition.getLinks();
   for ( std::size_t index = 0; index < links.size(); ++index )
   {
      const EntityLinkDefinition* plink = links[index];

      Entity& left = result->getChildren()[plink->getLeftIndex()];
      Entity& right = result->getChildren()[plink->getRightIndex()];

      QueryBodyComponentMessage message;
      left.sendComponentMessage(message);
      Body& leftbody = message.getBody();

      right.sendComponentMessage(message);
      Body& rightbody = message.getBody();

      leftbody.link(rightbody, plink->getJointDefinition());
   }

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

   result->setClassName(definition.getClassName());
   result->initialize();

   return result.release();
}
