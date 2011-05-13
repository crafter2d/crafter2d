
#include "core/math/vector.h"

#include "script/vm/virtualnativeobject.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobjectreference.h"

#include "script/scriptobject.h"
#include "script/scriptmanager.h"

#include "physics/inputforcegenerator.h"
#include "physics/box2d/box2dbody.h"
#include "physics/box2d/box2dsimulator.h"
#include "physics/box2d/box2drevolutejoint.h"

#include "net/bitstream.h"

#include "world/world.h"

#include "actionmap.h"
#include "keymap.h"
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

void Server_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   ActionMap* pmap = (ActionMap*) object.getObject();
   object.setOwned(false);

   pserver->setActionMap(pmap);
}

void Server_loadWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   std::string worldfile = accessor.getString(1);
   std::string objectname = accessor.getString(2);

   accessor.setResult(pserver->loadWorld(worldfile, objectname));
}

void Server_sendScriptEvent(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->asNative().getObject();

   BitStream* pstream = (BitStream*) accessor.getObject(1)->asNative().getObject();
   int client = accessor.getInt(2);

   pserver->sendScriptEvent(pstream, client);
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

void Client_getSceneGraph(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("SceneGraph", &pclient->getSceneGraph());
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void Client_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   ActionMap* pmap = (ActionMap*) object.getObject();
   object.setOwned(false);

   pclient->setActionMap(pmap);
}

void Client_setKeyMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   KeyMap* pmap = (KeyMap*) object.getObject();
   object.setOwned(false);

   pclient->setKeyMap(pmap);
}

void BitStream_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   BitStream* pstream = new BitStream();
   
   VirtualObjectReference ref = machine.instantiateNative("BitStream", pstream);

   accessor.setResult(ref);
}

void BitStream_writeInt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   BitStream* pstream = (BitStream*) thisobject->asNative().getObject();

   int value = accessor.getInt(1);

   pstream->writeInt(value);
}

void BitStream_readInt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   BitStream* pstream = (BitStream*) thisobject->asNative().getObject();

   accessor.setResult(pstream->readInt());
}

void BitStream_clear(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   BitStream* pstream = (BitStream*) thisobject->asNative().getObject();

   pstream->clear();
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

void Creature_getId(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   accessor.setResult((int)pcreature->getId());
}

void Creature_getPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("Vector2D", const_cast<Vector*>(&pcreature->getPosition()));
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void Creature_setPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   Vector* ppos = (Vector*) accessor.getObject(1)->asNative().getObject();

   pcreature->setPosition(*ppos);
}

void Creature_getVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   VirtualObjectReference ref = machine.instantiateNative("Vector2D", const_cast<Vector*>(&pcreature->getVelocity()));
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void Creature_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   Vector* ppos = (Vector*) accessor.getObject(1)->asNative().getObject();

   pcreature->setVelocity(*ppos);
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

void Creature_direction(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   accessor.setResult(pcreature->direction());
}

void Creature_flip(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->asNative().getObject();

   pcreature->flip();
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

void Player_getClient(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->asNative().getObject();

   accessor.setResult(pplayer->client);
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

void SceneGraph_find(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   SceneGraph* pgraph = (SceneGraph*) thisobject->asNative().getObject();

   int id = accessor.getInt(1);

   VirtualObjectReference ref = machine.instantiateNative("Creature", pgraph->find(id));
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void SceneGraph_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   SceneGraph* pgraph = (SceneGraph*) thisobject->asNative().getObject();

   Object* pobject = (Object*) accessor.getObject(1)->asNative().getObject();

   pgraph->setControler(pobject);
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

void World_setObjectLayer(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   int layer = accessor.getInt(1);

   pworld->setObjectLayer(layer);
}

void World_setFollowMode(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   int mode = accessor.getInt(1);

   pworld->setScrollMode(mode);
}

void World_setFollowObject(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   VirtualNativeObject& object = accessor.getObject(1)->asNative();
   Creature* pcreature = (Creature*) object.getObject();
   object.setOwned(false);

   pworld->setFollowObject(pcreature);
}

void World_getSimulator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   VirtualObjectReference& object = machine.instantiateNative("Box2DSimulator", &(Box2DSimulator&)pworld->getSimulator());
   object->asNative().setOwned(false);

   accessor.setResult(object);
}

void World_setFollowBorders(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->asNative().getObject();

   int left = accessor.getInt(1);
   int right = accessor.getInt(2);
   int top = accessor.getInt(3);
   int bottom = accessor.getInt(4);

   pworld->setFollowBorders(left, right, top, bottom);
}

void InputForceGenerator_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   InputForceGenerator* pgenerator = new InputForceGenerator();

   VirtualObjectReference ref = machine.instantiateNative("InputForceGenerator", pgenerator);

   accessor.setResult(ref);
}

void InputForceGenerator_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   InputForceGenerator* pgenerator = (InputForceGenerator*) thisobject->asNative().getObject();

   Vector* pvel = (Vector*) accessor.getObject(1)->asNative().getObject();

   pgenerator->setVelocity(*pvel);
}

void InputForceGenerator_setImpulse(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   InputForceGenerator* pgenerator = (InputForceGenerator*) thisobject->asNative().getObject();

   Vector* pvel = (Vector*) accessor.getObject(1)->asNative().getObject();

   pgenerator->setImpulse(*pvel);
}

void Box2DSimulator_createRevoluteJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DSimulator* psimulator = (Box2DSimulator*) thisobject->asNative().getObject();

   Box2DRevoluteJointDefinition* pjointdef = (Box2DRevoluteJointDefinition*) accessor.getObject(1)->asNative().getObject();
   
   psimulator->createRevoluteJoint(*pjointdef);
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

void Box2DRevoluteJointDefinition_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   Box2DRevoluteJointDefinition* pjointdef = new Box2DRevoluteJointDefinition();
   
   VirtualObjectReference ref = machine.instantiateNative("Box2DRevoluteJointDefinition", pjointdef);

   accessor.setResult(ref);
}

void Box2DRevoluteJointDefinition_getLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->asNative().getObject();

   VirtualObjectReference object = machine.instantiateNative("Box2DBody", pjoint->pleft);
   object->asNative().setOwned(false);

   accessor.setResult(object);
}

void Box2DRevoluteJointDefinition_setLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->asNative().getObject();

   VirtualObjectReference& left = accessor.getObject(1);

   pjoint->pleft = (Box2DBody*)left->asNative().getObject();
}

void Box2DRevoluteJointDefinition_getRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->asNative().getObject();

   VirtualObjectReference object = machine.instantiateNative("Box2DBody", pjoint->pright);
   object->asNative().setOwned(false);

   accessor.setResult(object);
}

void Box2DRevoluteJointDefinition_setRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->asNative().getObject();

   VirtualObjectReference& right = accessor.getObject(1);

   pjoint->pright = (Box2DBody*)right->asNative().getObject();
}

void Box2DRevoluteJointDefinition_getAnchor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->asNative().getObject();

   VirtualObjectReference object = machine.instantiateNative("Vector2D", &pjoint->anchor);
   object->asNative().setOwned(false);

   accessor.setResult(object);
}

void Box2DRevoluteJointDefinition_setAnchor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->asNative().getObject();

   VirtualObjectReference& anchor = accessor.getObject(1);

   pjoint->anchor = *(Vector*)anchor->asNative().getObject();
}

void ActionMap_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   ActionMap* pmap = new ActionMap();

   VirtualObjectReference ref = machine.instantiateNative("ActionMap", pmap);

   accessor.setResult(ref);
}

void KeyMap_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   KeyMap* pmap = new KeyMap();

   VirtualObjectReference ref = machine.instantiateNative("KeyMap", pmap);

   accessor.setResult(ref);
}

void KeyMap_bind(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   KeyMap* pmap = (KeyMap*) thisobject->asNative().getObject();

   int key = accessor.getInt(1);
   int action = accessor.getInt(2);

   pmap->bind(key, action);
}

// - Registration

void script_engine_register(ScriptManager& manager)
{
   ScriptRegistrator registrator;
   registrator.addCallback("Server_init", Server_init);
   registrator.addCallback("Server_create", Server_create);
   registrator.addCallback("Server_listen", Server_listen);
   registrator.addCallback("Server_update", Server_update);
   registrator.addCallback("Server_setActionMap", Server_setActionMap);
   registrator.addCallback("Server_loadWorld", Server_loadWorld);
   registrator.addCallback("Server_getSceneGraph", Server_getSceneGraph);
   registrator.addCallback("Server_sendScriptEvent", Server_sendScriptEvent);

   registrator.addCallback("Client_init", Client_init);
   registrator.addCallback("Client_create", Client_create);
   registrator.addCallback("Client_connect", Client_connect);
   registrator.addCallback("Client_update", Client_update);
   registrator.addCallback("Client_render", Client_render);
   registrator.addCallback("Client_getSceneGraph", Client_getSceneGraph);
   registrator.addCallback("Client_setActionMap", Client_setActionMap);
   registrator.addCallback("Client_setKeyMap", Client_setKeyMap);

   registrator.addCallback("BitStream_init", BitStream_init);
   registrator.addCallback("BitStream_writeInt", BitStream_writeInt);
   registrator.addCallback("BitStream_readInt", BitStream_readInt);
   registrator.addCallback("BitStream_clear", BitStream_clear);

   registrator.addCallback("Creature_init", Creature_init);
   registrator.addCallback("Creature_create", Creature_create);
   registrator.addCallback("Creature_getId", Creature_getId);
   registrator.addCallback("Creature_getPosition", Creature_getPosition);
   registrator.addCallback("Creature_setPosition", Creature_setPosition);
   registrator.addCallback("Creature_getVelocity", Creature_getVelocity);
   registrator.addCallback("Creature_setVelocity", Creature_setVelocity);
   registrator.addCallback("Creature_setName", Creature_setName);
   registrator.addCallback("Creature_setAnimation", Creature_setAnimation);
   registrator.addCallback("Creature_getBody", Creature_getBody);
   registrator.addCallback("Creature_direction", Creature_direction);
   registrator.addCallback("Creature_flip", Creature_flip);

   registrator.addCallback("Player_getName", Player_getName);
   registrator.addCallback("Player_getClient", Player_getClient);
   registrator.addCallback("Player_getController", Player_getController);
   registrator.addCallback("Player_setController", Player_setController);

   registrator.addCallback("Vector2D_init", Vector2D_init);
   registrator.addCallback("Vector2D_getX", Vector2D_getX);
   registrator.addCallback("Vector2D_setX", Vector2D_setX);
   registrator.addCallback("Vector2D_getY", Vector2D_getY);
   registrator.addCallback("Vector2D_setY", Vector2D_setY);

   registrator.addCallback("SceneGraph_setController", SceneGraph_setController);
   registrator.addCallback("SceneGraph_getWorld", SceneGraph_getWorld);
   registrator.addCallback("SceneGraph_find", SceneGraph_find);

   registrator.addCallback("World_init", World_init);
   registrator.addCallback("World_getName", World_getName);
   registrator.addCallback("World_getSceneGraph", World_getSceneGraph);
   registrator.addCallback("World_add", World_add);
   registrator.addCallback("World_setObjectLayer", World_setObjectLayer);
   registrator.addCallback("World_setFollowMode", World_setFollowMode);
   registrator.addCallback("World_setFollowObject", World_setFollowObject);
   registrator.addCallback("World_setFollowBorders", World_setFollowBorders);
   registrator.addCallback("World_getSimulator", World_getSimulator);

   registrator.addCallback("InputForceGenerator_init", InputForceGenerator_init);
   registrator.addCallback("InputForceGenerator_setVelocity", InputForceGenerator_setVelocity);
   registrator.addCallback("InputForceGenerator_setImpulse", InputForceGenerator_setImpulse);

   registrator.addCallback("Box2DSimulator_createRevoluteJoint", Box2DSimulator_createRevoluteJoint);

   registrator.addCallback("Box2DBody_addForceGenerator", Box2DBody_addForceGenerator);
   registrator.addCallback("Box2DBody_generateSensors", Box2DBody_generateSensors);

   registrator.addCallback("Box2DRevoluteJointDefinition_init", Box2DRevoluteJointDefinition_init);
   registrator.addCallback("Box2DRevoluteJointDefinition_getLeft", Box2DRevoluteJointDefinition_getLeft);
   registrator.addCallback("Box2DRevoluteJointDefinition_setLeft", Box2DRevoluteJointDefinition_setLeft);
   registrator.addCallback("Box2DRevoluteJointDefinition_getRight", Box2DRevoluteJointDefinition_getRight);
   registrator.addCallback("Box2DRevoluteJointDefinition_setRight", Box2DRevoluteJointDefinition_setRight);
   registrator.addCallback("Box2DRevoluteJointDefinition_getAnchor", Box2DRevoluteJointDefinition_getAnchor);
   registrator.addCallback("Box2DRevoluteJointDefinition_setAnchor", Box2DRevoluteJointDefinition_setAnchor);

   registrator.addCallback("ActionMap_init", ActionMap_init);

   registrator.addCallback("KeyMap_init", KeyMap_init);
   registrator.addCallback("KeyMap_bind", KeyMap_bind);

   registrator.registerCallbacks(manager);
}
