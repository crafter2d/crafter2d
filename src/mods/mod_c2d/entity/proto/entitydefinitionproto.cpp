
#include "entitydefinitionproto.h"

#include "core/content/contentmanager.h"
#include "core/entity/entity.h"
#include "core/physics/revolutejointdefinition.h"
#include "core/entity/componentmessages/querybodycomponentmessage.h"
#include "core/entity/components/netcomponent.h"
#include "core/streams/datastream.h"
#include "core/defines.h"

#include "../factories/componentfactories.h"

#include "childdefinitionproto.h"
#include "componentdefinitionproto.h"
#include "linkdefinitionproto.h"


// - Instantiation

Entity* EntityDefinitionProto::instantiate(c2d::ContentManager& manager, ComponentFactories& factories)
{
   Entity* pentity = new Entity();
   pentity->setName(mName);
   pentity->setClassName(mClassName);

   // child entities

   for ( uint32_t index = 0; index < mChildren.size(); ++index )
   {
      ChildDefinitionProto* pchilddef = mChildren[index];

      Entity* pchild = NULL;
      switch ( pchilddef->mRefType )
      {
      case ChildDefinitionProto::eFileReference:
         {
            pchild = manager.loadContent<Entity>(pchilddef->mRef);
         }
         break;

      case ChildDefinitionProto::eLocalReference:
         {
            EntityDefinitionProto& def = resolve(pchilddef->mRef);
            pchild = def.instantiate(manager, factories);
         }
         break;
      }
      
      ASSERT_PTR(pchild);
      pchild->setOffset(pchilddef->mOffset);
      pentity->add(pchild);
   }

   // link children

   for ( uint32_t index = 0; index < mLinks.size(); ++index )
   {
      LinkDefinitionProto* plinkdef = mLinks[index];

      Entity& leftchild = pentity->getChildren()[plinkdef->mLeft - 1];
      Entity& rightchild = pentity->getChildren()[plinkdef->mRight - 1];

      QueryBodyComponentMessage message;
      leftchild.sendComponentMessage(message);
      Body& leftbody = message.getBody();

      rightchild.sendComponentMessage(message);
      Body& rightbody = message.getBody();

      leftbody.link(rightbody, *plinkdef->mpJointDef);
   }

   // components

   for ( uint32_t index = 0; index < mComponents.size(); ++index )
   {
      ComponentDefinitionProto* pcomponentdef = mComponents[index];
      Component* pcomponent = factories.instantiate(*pcomponentdef);
      if ( pcomponent != NULL )
      {
         pentity->addComponent(pcomponent);
      }
   }

   pentity->initialize();
   pentity->addComponent(new NetComponent());

   return pentity;
}

// - Storage

void EntityDefinitionProto::read(DataStream& stream)
{
   uint32_t size;

   stream >> mName >> mClassName;

   stream.readUint(size);
   for ( uint32_t index = 0; index < size; ++index )
   {
      ChildDefinitionProto* pchild = new ChildDefinitionProto();
      pchild->read(stream);
      mChildren.push_back(pchild);
   }

   stream.readUint(size);
   for ( uint32_t index = 0; index < size; ++index )
   {
      EntityDefinitionProto* pentity = new EntityDefinitionProto();
      pentity->read(stream);
      mEntities.push_back(pentity);
   }

   stream.readUint(size);
   for ( uint32_t index = 0; index < size; ++index )
   {
      LinkDefinitionProto* plink = new LinkDefinitionProto();
      plink->read(stream);
      mLinks.push_back(plink);
   }

   stream.readUint(size);
   for ( uint32_t index = 0; index < size; ++index )
   {
      ComponentDefinitionProto* pcomponent = ComponentDefinitionProto::fromStream(stream);
      mComponents.push_back(pcomponent);
   }
}

void EntityDefinitionProto::write(DataStream& stream) const
{
   stream << mName << mClassName;

   stream.writeUint(mChildren.size());
   for ( std::size_t index = 0; index < mChildren.size(); ++index )
   {
      const ChildDefinitionProto* pchild = mChildren[index];
      pchild->write(stream);
   }

   stream.writeUint(mEntities.size());
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      const EntityDefinitionProto* pentity = mEntities[index];
      pentity->write(stream);
   }

   stream.writeUint(mLinks.size());
   for ( std::size_t index = 0; index < mLinks.size(); ++index )
   {
      const LinkDefinitionProto* plink = mLinks[index];
      plink->write(stream);
   }

   stream.writeUint(mComponents.size());
   for ( std::size_t index = 0; index < mComponents.size(); ++index )
   {
      const ComponentDefinitionProto* pcomponent = mComponents[index];
      pcomponent->write(stream);
   }
}

// - Search

EntityDefinitionProto& EntityDefinitionProto::resolve(const String& name)
{
   for ( uint32_t index = 0; index < mEntities.size(); ++index )
   {
      EntityDefinitionProto* pentity = mEntities[index];
      if ( pentity->mName == name )
      {
         return *pentity;
      }
   }

   UNREACHABLE("Could not resolve entity");
}
