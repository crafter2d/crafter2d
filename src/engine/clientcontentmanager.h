
#ifndef CLIENT_CONTENT_MANANGER_H
#define CLIENT_CONTENT_MANANGER_H

#include "engine/content/contentmanager.h"

class Client;

class ClientContentManager : public ContentManager
{
public:
   explicit ClientContentManager(Client& process);

   virtual Entity*  loadEntity(const String& filename);
   virtual World*   loadWorld(const String& filename);
};

#endif // CLIENT_CONTENT_MANANGER_H
