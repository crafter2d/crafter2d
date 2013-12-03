
#include "entityreader.h"

#include "core/entity/entity.h"
#include "core/defines.h"

#include "factories/componentfactories.h"
#include "factories/meshcomponentfactory.h"
#include "factories/physicscomponentfactory.h"

#include "proto/entitydefinitionproto.h"

IContent* EntityReader::read(DataStream& stream)
{
   ComponentFactories factories;
   setupFactories(factories);

   EntityDefinitionProto* pentityproto = new EntityDefinitionProto();
   pentityproto->read(stream);

   return pentityproto->instantiate(factories);
}

void EntityReader::setupFactories(ComponentFactories& factories)
{
   if ( hasGraphicsDevice() )
   {
      factories.push_back(new MeshComponentFactory(getGraphicsDevice()));
   }

   if ( hasPhysicsSimulator() )
   {
      factories.push_back(new PhysicsComponentFactory(getPhysicsSimulator()));
   }
}
