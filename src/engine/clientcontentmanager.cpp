
#include "clientcontentmanager.h"

#include "content/contentloader.h"
#include "client.h"
#include "entity.h"

ClientContentManager::ClientContentManager(Client& process):
   ContentManager(process)
{
}

// - Overloads
   
Entity* ClientContentManager::loadEntity(const String& filename)
{
   Entity* pentity = doLoadEntity(filename, ContentLoader::eLoadGraphics | ContentLoader::eLoadPhysics);
   pentity->initialize(static_cast<Client&>(getProcess()).getDevice());

   return pentity;
}

World* ClientContentManager::loadWorld(const String& filename)
{
   return ContentManager::doLoadWorld(filename, ContentLoader::eLoadPhysics);
}
