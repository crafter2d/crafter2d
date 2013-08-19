
#ifndef SERVER_CONTENT_MANAGER_H
#define SERVER_CONTENT_MANAGER_H

#include "engine/content/contentmanager.h"

class Server;

class ServerContentManager : public ContentManager
{
public:
   ServerContentManager(Server& server);

protected:
 // overrides
   virtual void initializeEntityFactory(EntityFactory& factory) override;
};

#endif // SERVER_CONTENT_MANAGER_H
