
#ifndef CLIENT_CONTENT_MANANGER_H
#define CLIENT_CONTENT_MANANGER_H

#include "engine/content/contentmanager.h"

class Client;
class EntityFactory;

class ClientContentManager : public ContentManager
{
public:
   explicit ClientContentManager(Client& process);

protected:
 // overrides
   virtual void initializeEntityFactory(EntityFactory& factory) override;
};

#endif // CLIENT_CONTENT_MANANGER_H
