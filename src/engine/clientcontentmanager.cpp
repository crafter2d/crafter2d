
#include "clientcontentmanager.h"

#include "content/contentloader.h"
#include "components/physicscomponentfactory.h"
#include "components/meshcomponentfactory.h"
#include "world/world.h"

#include "client.h"
#include "entity.h"

ClientContentManager::ClientContentManager(Client& process):
   ContentManager(process)
{
}

// - Overrides

void ClientContentManager::initializeEntityFactory(EntityFactory& factory)
{
   factory.registerComponentFactory(new PhysicsComponentFactory(getProcess().getWorld().getSimulator()));
   factory.registerComponentFactory(new MeshComponentFactory(static_cast<Client&>(getProcess()).getDevice()));
}
