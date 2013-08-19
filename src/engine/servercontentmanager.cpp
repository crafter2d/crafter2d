
#include "servercontentmanager.h"

#include "engine/content/contentloader.h"
#include "engine/components/physicscomponentfactory.h"
#include "engine/world/world.h"

#include "server.h"

ServerContentManager::ServerContentManager(Server& server):
   ContentManager(server)
{

}

// - Overrides

void ServerContentManager::initializeEntityFactory(EntityFactory& factory)
{
   factory.registerComponentFactory(new PhysicsComponentFactory(getProcess().getWorld().getSimulator()));
}
