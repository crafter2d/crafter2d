
#include "core/math/vector.h"

#include "script/vm/virtualnativeobject.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobjectreference.h"

#include "script/scriptobject.h"
#include "script/scriptmanager.h"

#include "physics/inputforcegenerator.h"
#include "physics/box2d/box2dbody.h"

#include "world/world.h"

#include "client.h"
#include "creature.h"
#include "player.h"
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

void Server_getSceneGraph(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("SceneGraph", &pserver->getSceneGraph());
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
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

void Creature_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   Creature* pcreature = new Creature();

   VirtualObjectReference ref = machine.instantiateNative("Creature", pcreature);

   accessor.setResult(ref);
}

void Creature_create(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   World* pworld = (World*) accessor.getObject(1)->asNative().getObject();
   const std::string& file = accessor.getString(2);

   accessor.setResult(pcreature->create(*pworld, file));
}

void Creature_setPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   Vector* ppos = (Vector*) accessor.getObject(1)->asNative().getObject();

   pcreature->setPosition(*ppos);
}

void Creature_setName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   const std::string& name = accessor.getString(1);

   pcreature->setName(name);
}

void Creature_setAnimation(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   int index = accessor.getInt(1);

   pcreature->setAnimation(index);
}

void Creature_getBody(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("Box2DBody", &pcreature->getBody());
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void Player_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->asNative().getObject();

   accessor.setResult(pplayer->getName());
}

void Player_getController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("Creature", pplayer->controler);
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void Player_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   Creature* pcreature = (Creature*) object.getObject();
   object.setOwned(false);

   pplayer->controler = pcreature;
}

void Vector2D_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   Vector* pvector = new Vector();

   VirtualObjectReference ref = machine.instantiateNative("Vector2D", pvector);

   accessor.setResult(ref);
}

void Vector2D_getX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->asNative().getObject();

   accessor.setResult(pvector->x);
}

void Vector2D_setX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->asNative().getObject();

   float x = accessor.getReal(1);

   pvector->x = x;
}

void Vector2D_getY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->asNative().getObject();

   accessor.setResult(pvector->y);
}

void Vector2D_setY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->asNative().getObject();

   float y = accessor.getReal(1);

   pvector->y = y;
}

void SceneGraph_getWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   SceneGraph* pgraph = (SceneGraph*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("World", pgraph->getWorld());
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void World_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   World* pworld = new World();
   
   VirtualObjectReference ref = machine.instantiateNative("World", pworld);

   accessor.setResult(ref);
}

void World_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   accessor.setResult(pworld->getName());
}

void World_getSceneGraph(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("SceneGraph", &pworld->getSceneGraph());
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void World_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   Creature* pcreature = (Creature*) object.getObject();
   object.setOwned(false);

   pworld->add(pcreature);
}

void InputForceGenerator_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   InputForceGenerator* pgenerator = new InputForceGenerator();

   VirtualObjectReference ref = machine.instantiateNative("InputForceGenerator", pgenerator);

   accessor.setResult(ref);
}

void Box2DBody_addForceGenerator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DBody* pbody = (Box2DBody*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   InputForceGenerator* pgenerator = (InputForceGenerator*) object.getObject();
   object.setOwned(false);

   pbody->addForceGenerator(pgenerator);
}

void Box2DBody_generateSensors(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DBody* pbody = (Box2DBody*) thisobject->asNative().getObject();

   pbody->generateSensors();
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
   registrator.addCallback("Server_getSceneGraph", Server_getSceneGraph);

   registrator.addCallback("Client_init", Client_init);
   registrator.addCallback("Client_create", Client_create);
   registrator.addCallback("Client_connect", Client_connect);
   registrator.addCallback("Client_update", Client_update);
   registrator.addCallback("Client_render", Client_render);

   registrator.addCallback("Creature_init", Creature_init);
   registrator.addCallback("Creature_create", Creature_create);
   registrator.addCallback("Creature_setPosition", Creature_setPosition);
   registrator.addCallback("Creature_setName", Creature_setName);
   registrator.addCallback("Creature_setAnimation", Creature_setAnimation);
   registrator.addCallback("Creature_getBody", Creature_getBody);

   registrator.addCallback("Player_getName", Player_getName);
   registrator.addCallback("Player_getController", Player_getController);
   registrator.addCallback("Player_setController", Player_setController);

   registrator.addCallback("Vector2D_init", Vector2D_init);
   registrator.addCallback("Vector2D_getX", Vector2D_getX);
   registrator.addCallback("Vector2D_setX", Vector2D_setX);
   registrator.addCallback("Vector2D_getY", Vector2D_getY);
   registrator.addCallback("Vector2D_setY", Vector2D_setY);

   registrator.addCallback("SceneGraph_getWorld", SceneGraph_getWorld);

   registrator.addCallback("World_init", World_init);
   registrator.addCallback("World_getName", World_getName);
   registrator.addCallback("World_getSceneGraph", World_getSceneGraph);
   registrator.addCallback("World_add", World_add);

   registrator.addCallback("InputForceGenerator_init", InputForceGenerator_init);

   registrator.addCallback("Box2DBody_addForceGenerator", Box2DBody_addForceGenerator);
   registrator.addCallback("Box2DBody_generateSensors", Box2DBody_generateSensors);

   registrator.registerCallbacks(manager);
}
