
#ifndef SERVER_CONTENT_MANAGER_H
#define SERVER_CONTENT_MANAGER_H

#include "engine/content/contentmanager.h"

class Server;

class ServerContentManager : public ContentManager
{
public:
   ServerContentManager(Server& server);

 // loading
   virtual Entity*  loadEntity(const String& filename);
   virtual World*   loadWorld(const String& filename);
};

#endif // SERVER_CONTENT_MANAGER_H
