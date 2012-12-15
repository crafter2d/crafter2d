
#include "servercontentmanager.h"

#include "engine/content/contentloader.h"

#include "server.h"

ServerContentManager::ServerContentManager(Server& server):
   ContentManager(server)
{
}

// - Loading

Entity* ServerContentManager::loadEntity(const String& filename)
{
   return doLoadEntity(filename, ContentLoader::eLoadPhysics);
}

World* ServerContentManager::loadWorld(const String& filename)
{
   return doLoadWorld(filename, ContentLoader::eLoadPhysics);
}
