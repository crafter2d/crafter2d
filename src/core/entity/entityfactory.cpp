
#include "entityfactory.h"

#include "core/physics/revolutejointdefinition.h"
#include "core/smartptr/autoptr.h"

#include "core/entity/entity.h"
#include "core/entity/components/componentfactory.h"
#include "core/entity/componentmessages/querybodycomponentmessage.h"

#include "entitydefinition.h"
#include "entitylinkdefinition.h"


// PROBABLY GOING TO BE REMOVED AS THE MOD WILL DO THE INSTANTIATION AND LOADING OF THE ENTITIES
// otherwise the exercise of moving this to core wasn't necessary ;-)


EntityFactory::EntityFactory():
   mFactories(),
   mDefinitions()
{
}

void EntityFactory::registerComponentFactory(ComponentFactory* pfactory)
{
   mFactories.insert(std::make_pair(pfactory->getComponentType(), pfactory));
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
      //presult = mLoader.load(filename);
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
