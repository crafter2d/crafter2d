
#include "script/vm/virtualnativeobject.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobjectreference.h"

#include "script/scriptobject.h"
#include "script/scriptmanager.h"

#include "client.h"
#include "server.h"

void Server_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   Server* pserver = new Server();

   VirtualObjectReference ref = machine.instantiateNative("Server", pserver);

   accessor.setResult(ref);
}

void Server_create(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   accessor.setResult(pserver->create());
}

void Server_listen(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   int port = accessor.getInt(1);

   pserver->listen(port);
}

void Server_loadWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   std::string worldfile = accessor.getString(1);
   std::string objectname = accessor.getString(2);

   accessor.setResult(pserver->loadWorld(worldfile, objectname));
}

void Server_update(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   float delta = accessor.getReal(1);

   pserver->update(delta);
}

void Client_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   Client* pclient = new Client();

   VirtualObjectReference ref = machine.instantiateNative("Client", pclient);

   accessor.setResult(ref);
}

void Client_create(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   accessor.setResult(pclient->create());
}

void Client_connect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   std::string ip = accessor.getString(1);
   int port = accessor.getInt(2);
   std::string name = accessor.getString(3);

   pclient->connect(ip.c_str(), port, name.c_str());
}

void Client_update(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   float delta = accessor.getReal(1);

   pclient->update(delta);
}

void Client_render(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   float delta = accessor.getReal(1);

   pclient->render(delta);
}

// - Registration

void script_engine_register(ScriptManager& manager)
{
   ScriptRegistrator registrator;
   registrator.addCallback("Server_init", Server_init);
   registrator.addCallback("Server_create", Server_create);
   registrator.addCallback("Server_listen", Server_listen);
   registrator.addCallback("Server_update", Server_update);
   registrator.addCallback("Server_loadWorld", Server_loadWorld);

   registrator.addCallback("Client_init", Client_init);
   registrator.addCallback("Client_create", Client_create);
   registrator.addCallback("Client_connect", Client_connect);
   registrator.addCallback("Client_update", Client_update);
   registrator.addCallback("Client_render", Client_render);

   registrator.registerCallbacks(manager);
}
