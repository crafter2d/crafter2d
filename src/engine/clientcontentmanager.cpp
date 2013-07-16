
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
   Client& client = static_cast<Client&>(getProcess());
   Entity* pentity = doLoadEntity(filename, ContentLoader::eLoadGraphics | ContentLoader::eLoadPhysics);
   pentity->initialize(client.getDevice(), client.getRenderContext());

   return pentity;
}

World* ClientContentManager::loadWorld(const String& filename)
{
   return ContentManager::doLoadWorld(filename, ContentLoader::eLoadPhysics);
}
