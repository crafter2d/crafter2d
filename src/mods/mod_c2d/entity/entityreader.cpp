
#include "entityreader.h"

#include <memory>

#include "core/entity/entity.h"
#include "core/defines.h"

#include "factories/componentfactories.h"
#include "factories/meshcomponentfactory.h"
#include "factories/particlecomponentfactory.h"
#include "factories/physicscomponentfactory.h"

#include "proto/entitydefinitionproto.h"

IContent* EntityReader::read(DataStream& stream)
{
   ComponentFactories factories;
   setupFactories(factories);

   auto pentityproto = std::make_unique<EntityDefinitionProto>();
   pentityproto->read(stream);

   return pentityproto->instantiate(getContentManager(), factories);
}

void EntityReader::setupFactories(ComponentFactories& factories)
{
   if ( hasGraphicsDevice() )
   {
      factories.push_back(new MeshComponentFactory(getGraphicsDevice()));
      factories.push_back(new ParticleComponentFactory(getGraphicsDevice()));
   }

   if ( hasPhysicsSimulator() )
   {
      factories.push_back(new PhysicsComponentFactory(getPhysicsSimulator()));
   }
}
