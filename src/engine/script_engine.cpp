/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "core/math/vector.h"

#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobjectreference.h"

#include "script/scriptobject.h"
#include "script/scriptmanager.h"

#include "physics/inputforcegenerator.h"
#include "physics/box2d/box2dbody.h"
#include "physics/box2d/box2dsimulator.h"
#include "physics/box2d/box2drevolutejoint.h"

#include "ui/graphics.h"
#include "ui/font.h"

#include "resource/resourcemanager.h"

#include "window/gamewindowfactory.h"

#include "net/bitstream.h"

#include "world/world.h"

#include "actionmap.h"
#include "keymap.h"
#include "client.h"
#include "creature.h"
#include "player.h"
#include "server.h"
#include "inputcontroller.h"

void Process_create(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Process* pprocess = (Process*) thisobject->getNativeObject();

   const std::string& name = accessor.getString(1);

   accessor.setResult(pprocess->create(name));
}

void Process_getSceneGraph(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Process* pprocess = (Process*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("SceneGraph", &pprocess->getSceneGraph(), false);

   accessor.setResult(ref);
}

void Process_setScriptManager(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Process* pprocess = (Process*) thisobject->getNativeObject();

   ScriptManager* pscriptmanager = (ScriptManager*) accessor.getObject(1)->useNativeObject();

   pprocess->setScriptManager(pscriptmanager);
}

void Process_setObject(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Process* pprocess = (Process*) thisobject->getNativeObject();

   VirtualObjectReference& object = accessor.getObject(1);

   pprocess->setObject(object);
}

void Process_getFont(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Process* pprocess = (Process*) thisobject->getNativeObject();

   const std::string& name = accessor.getString(1);
   int size = accessor.getInt(2);

   FontPtr* pfont = new FontPtr(ResourceManager::getInstance().getFont(name, size));
   accessor.setResult(machine.instantiateNative("engine.ui.Font", pfont, true)); // take ownership of this handle
}

void Server_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   
   Server* pserver = new Server();
   machine.registerNative(thisobject, pserver);
}

void Server_listen(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->getNativeObject();

   int port = accessor.getInt(1);

   pserver->listen(port);
}

void Server_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->getNativeObject();

   ActionMap* pmap = (ActionMap*) accessor.getObject(1)->useNativeObject();

   pserver->setActionMap(pmap);
}

void Server_loadWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->getNativeObject();

   std::string worldfile = accessor.getString(1);
   std::string objectname = accessor.getString(2);

   accessor.setResult(pserver->loadWorld(worldfile, objectname));
}

void Server_sendScriptEvent(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->getNativeObject();

   BitStream* pstream = (BitStream*) accessor.getObject(1)->getNativeObject();
   int client = accessor.getInt(2);

   pserver->sendScriptEvent(pstream, client);
}

void Server_update(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Server* pserver = (Server*) thisobject->getNativeObject();

   float delta = accessor.getReal(1);

   pserver->update(delta);
}

void Client_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   Client* pclient = new Client();
   machine.registerNative(thisobject, pclient);
}

void Client_connect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   std::string ip = accessor.getString(1);
   int port = accessor.getInt(2);
   std::string name = accessor.getString(3);

   pclient->connect(ip.c_str(), port, name.c_str());
}

void Client_update(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   float delta = accessor.getReal(1);

   pclient->update(delta);
}

void Client_nativeRender(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   float delta = accessor.getReal(1);

   pclient->render(delta);
}

void Client_nativeDisplay(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();
   
   pclient->display();
}

void Client_isActive(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   accessor.setResult(pclient->isActive());
}

void Client_setWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   GameWindow* pwindow = (GameWindow*) accessor.getObject(1)->useNativeObject();
   
   pclient->setWindow(pwindow);
}

void Client_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   ActionMap* pmap = (ActionMap*) accessor.getObject(1)->useNativeObject();

   pclient->setActionMap(pmap);
}

void Client_setKeyMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Client* pclient = (Client*) thisobject->getNativeObject();

   KeyMap* pmap = (KeyMap*) accessor.getObject(1)->useNativeObject();

   pclient->setKeyMap(pmap);
}

void ScriptManager_spawnChild(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   ScriptManager* pscriptmanager = (ScriptManager*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("ScriptManager", pscriptmanager->spawnChild());

   accessor.setResult(ref);
}

void GameWindowFactory_createWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   GameWindowFactory* pfactory = (GameWindowFactory*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("System.GameWindow", pfactory->createWindow());

   accessor.setResult(ref);
}

void BitStream_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   BitStream* pstream = new BitStream();
   machine.registerNative(thisobject, pstream);
}

void BitStream_writeInt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   BitStream* pstream = (BitStream*) thisobject->getNativeObject();

   int value = accessor.getInt(1);

   pstream->writeInt(value);
}

void BitStream_readInt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   BitStream* pstream = (BitStream*) thisobject->getNativeObject();

   accessor.setResult(pstream->readInt());
}

void BitStream_clear(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   BitStream* pstream = (BitStream*) thisobject->getNativeObject();

   pstream->clear();
}

void Creature_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   Creature* pcreature = new Creature();
   machine.registerNative(thisobject, pcreature);
}

void Creature_create(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();
   thisobject->setOwner(false);

   World* pworld = (World*) accessor.getObject(1)->getNativeObject();
   const std::string& file = accessor.getString(2);

   accessor.setResult(pcreature->create(*pworld, file));
}

void Creature_getId(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   accessor.setResult((int)pcreature->getId());
}

void Creature_getPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("Vector2D", const_cast<Vector*>(&pcreature->getPosition()), false);

   accessor.setResult(ref);
}

void Creature_setPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   Vector* ppos = (Vector*) accessor.getObject(1)->getNativeObject();

   pcreature->setPosition(*ppos);
}

void Creature_getVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("Vector2D", const_cast<Vector*>(&pcreature->getVelocity()), false);

   accessor.setResult(ref);
}

void Creature_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   Vector* ppos = (Vector*) accessor.getObject(1)->getNativeObject();

   pcreature->setVelocity(*ppos);
}

void Creature_setName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   const std::string& name = accessor.getString(1);

   pcreature->setName(name);
}

void Creature_setAnimation(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   int index = accessor.getInt(1);

   pcreature->setAnimation(index);
}

void Creature_direction(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   accessor.setResult(pcreature->direction());
}

void Creature_flip(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   pcreature->flip();
}

void Creature_getBody(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("box2d.Box2DBody", &pcreature->getBody(), false);

   accessor.setResult(ref);
}

void Creature_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Creature* pcreature = (Creature*) thisobject->getNativeObject();

   InputController* pcontroller = (InputController*) accessor.getObject(1)->useNativeObject();

   pcreature->setController(pcontroller);
}

void Player_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->getNativeObject();

   accessor.setResult(pplayer->getName());
}

void Player_getClient(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->getNativeObject();

   accessor.setResult(pplayer->client);
}

void Player_getController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("Creature", pplayer->controler, false);

   accessor.setResult(ref);
}

void Player_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Player* pplayer = (Player*) thisobject->getNativeObject();

   Creature* pcreature = (Creature*) accessor.getObject(1)->getNativeObject();

   pplayer->controler = pcreature;
}

void Vector2D_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   Vector* pvector = new Vector();
   machine.registerNative(thisobject, pvector);
}

void Vector2D_getX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->getNativeObject();

   accessor.setResult(pvector->x);
}

void Vector2D_setX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->getNativeObject();

   float x = accessor.getReal(1);

   pvector->x = x;
}

void Vector2D_getY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->getNativeObject();

   accessor.setResult(pvector->y);
}

void Vector2D_setY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Vector* pvector = (Vector*) thisobject->getNativeObject();

   float y = accessor.getReal(1);

   pvector->y = y;
}

void SceneGraph_getWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   SceneGraph* pgraph = (SceneGraph*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("World", pgraph->getWorld(), false);

   accessor.setResult(ref);
}

void SceneGraph_find(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   SceneGraph* pgraph = (SceneGraph*) thisobject->getNativeObject();

   int id = accessor.getInt(1);

   VirtualObjectReference ref = machine.instantiateNative("Creature", pgraph->find(id), false);

   accessor.setResult(ref);
}

void SceneGraph_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   SceneGraph* pgraph = (SceneGraph*) thisobject->getNativeObject();

   Object* pobject = (Object*) accessor.getObject(1)->getNativeObject();

   pgraph->setControler(pobject);
}

void World_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   World* pworld = new World();
   machine.registerNative(thisobject, pworld);
}

void World_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   accessor.setResult(pworld->getName());
}

void World_getSceneGraph(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   VirtualObjectReference ref = machine.instantiateNative("SceneGraph", &pworld->getSceneGraph(), false);

   accessor.setResult(ref);
}

void World_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   Creature* pcreature = (Creature*) accessor.getObject(1)->useNativeObject();

   pworld->add(pcreature);
}

void World_setObjectLayer(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   int layer = accessor.getInt(1);

   pworld->setObjectLayer(layer);
}

void World_setFollowMode(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   int mode = accessor.getInt(1);

   pworld->setScrollMode(mode);
}

void World_setFollowObject(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   Creature* pcreature = (Creature*) accessor.getObject(1)->getNativeObject();
   
   pworld->setFollowObject(pcreature);
}

void World_getSimulator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   VirtualObjectReference& object = machine.instantiateNative("box2d.Box2DSimulator", &(Box2DSimulator&)pworld->getSimulator(), false);

   accessor.setResult(object);
}

void World_setFollowBorders(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   World* pworld = (World*) thisobject->getNativeObject();

   int left = accessor.getInt(1);
   int right = accessor.getInt(2);
   int top = accessor.getInt(3);
   int bottom = accessor.getInt(4);

   pworld->setFollowBorders(left, right, top, bottom);
}

void InputForceGenerator_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   InputForceGenerator* pgenerator = new InputForceGenerator();
   machine.registerNative(thisobject, pgenerator);
}

void InputForceGenerator_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   InputForceGenerator* pgenerator = (InputForceGenerator*) thisobject->getNativeObject();

   Vector* pvel = (Vector*) accessor.getObject(1)->getNativeObject();

   pgenerator->setVelocity(*pvel);
}

void InputForceGenerator_setImpulse(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   InputForceGenerator* pgenerator = (InputForceGenerator*) thisobject->getNativeObject();

   Vector* pvel = (Vector*) accessor.getObject(1)->getNativeObject();

   pgenerator->setImpulse(*pvel);
}

void InputController_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   InputController* pcontroller = new InputController();
   machine.registerNative(thisobject, pcontroller);
}

void InputController_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   InputController* pcontroller = (InputController*) thisobject->getNativeObject();

   ActionMap* pactionmap = (ActionMap*) accessor.getObject(1)->useNativeObject();

   pcontroller->setActionMap(pactionmap);
}

void Box2DSimulator_createRevoluteJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DSimulator* psimulator = (Box2DSimulator*) thisobject->getNativeObject();

   Box2DRevoluteJointDefinition* pjointdef = (Box2DRevoluteJointDefinition*) accessor.getObject(1)->getNativeObject();
   
   psimulator->createRevoluteJoint(*pjointdef);
}

void Box2DBody_addForceGenerator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DBody* pbody = (Box2DBody*) thisobject->getNativeObject();

   InputForceGenerator* pgenerator = (InputForceGenerator*) accessor.getObject(1)->useNativeObject();
   
   pbody->addForceGenerator(pgenerator);
}

void Box2DBody_generateSensors(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DBody* pbody = (Box2DBody*) thisobject->getNativeObject();

   pbody->generateSensors();
}

void Box2DRevoluteJointDefinition_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   Box2DRevoluteJointDefinition* pjointdef = new Box2DRevoluteJointDefinition();
   machine.registerNative(thisobject, pjointdef);
}

void Box2DRevoluteJointDefinition_getLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->getNativeObject();

   VirtualObjectReference object = machine.instantiateNative("box2d.Box2DBody", pjoint->pleft, false);

   accessor.setResult(object);
}

void Box2DRevoluteJointDefinition_setLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->getNativeObject();

   VirtualObjectReference& left = accessor.getObject(1);

   pjoint->pleft = (Box2DBody*)left->getNativeObject();
}

void Box2DRevoluteJointDefinition_getRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->getNativeObject();

   VirtualObjectReference object = machine.instantiateNative("box2d.Box2DBody", pjoint->pright, false);

   accessor.setResult(object);
}

void Box2DRevoluteJointDefinition_setRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->getNativeObject();

   VirtualObjectReference& right = accessor.getObject(1);

   pjoint->pright = (Box2DBody*)right->getNativeObject();
}

void Box2DRevoluteJointDefinition_getAnchor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->getNativeObject();

   VirtualObjectReference object = machine.instantiateNative("Vector2D", &pjoint->anchor, false);

   accessor.setResult(object);
}

void Box2DRevoluteJointDefinition_setAnchor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Box2DRevoluteJointDefinition* pjoint = (Box2DRevoluteJointDefinition*) thisobject->getNativeObject();

   pjoint->anchor = *(Vector*)accessor.getObject(1)->getNativeObject();
}

void ActionMap_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   ActionMap* pmap = new ActionMap();
   machine.registerNative(thisobject, pmap);
}

void ActionMap_setProcess(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   ActionMap* pmap = (ActionMap*) thisobject->getNativeObject();

   Process* pprocess = (Process*) accessor.getObject(1)->getNativeObject();

   pmap->setProcess(*pprocess);
}

void KeyMap_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   KeyMap* pmap = new KeyMap();
   machine.registerNative(thisobject, pmap);
}

void KeyMap_bind(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   KeyMap* pmap = (KeyMap*) thisobject->getNativeObject();

   int key = accessor.getInt(1);
   int action = accessor.getInt(2);

   pmap->bind(key, action);
}

void EngineGraphics_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();

   Graphics* pgraphics = new Graphics();
   machine.registerNative(thisobject, pgraphics);
}

void EngineGraphics_doSetColor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   float red = accessor.getReal(1);
   float green = accessor.getReal(2);
   float blue = accessor.getReal(3);
   float alpha = accessor.getReal(4);

   pgraphics->setColor(red, green, blue, alpha);
}

void EngineGraphics_nativeSetFont(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   FontPtr* pfont = (FontPtr*)accessor.getObject(1)->getNativeObject();

   pgraphics->setFont(*(pfont->ptr()));
}

void EngineGraphics_translate(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);

   pgraphics->translate(x, y);
}

void EngineGraphics_drawText(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   const std::string& text = accessor.getString(3);

   pgraphics->drawText(x, y, text);
}

void EngineGraphics_native_fillRect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   pgraphics->fillRect(x, y, width, height);
}

void EngineGraphics_native_drawRect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   pgraphics->drawRect(x, y, width, height);
}

void EngineGraphics_native_drawRoundedRect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Graphics* pgraphics = (Graphics*) thisobject->getNativeObject();

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   pgraphics->drawRoundedRect(x, y, width, height);
}

void Font_render(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   FontPtr* pfont = (FontPtr*) thisobject->getNativeObject();

   const std::string& text = accessor.getString(1);

   (*pfont)->render(text);
}

void Font_native_textWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   FontPtr* pfont = (FontPtr*) thisobject->getNativeObject();

   const std::string& text = accessor.getString(1);

   accessor.setResult((*pfont)->getTextWidth(text));
}

void Font_native_textHeight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   FontPtr* pfont = (FontPtr*) thisobject->getNativeObject();

   const std::string& text = accessor.getString(1);

   accessor.setResult((*pfont)->getTextHeight(text));
}

void Font_getBaseLine(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   FontPtr* pfont = (FontPtr*) thisobject->getNativeObject();

   accessor.setResult((*pfont)->getBaseLine());
}

// - Registration

void script_engine_register(ScriptManager& manager)
{
   ScriptRegistrator registrator;

   registrator.addCallback("Process_create", Process_create);
   registrator.addCallback("Process_getSceneGraph", Process_getSceneGraph);
   registrator.addCallback("Process_setScriptManager", Process_setScriptManager);
   registrator.addCallback("Process_setObject", Process_setObject);
   registrator.addCallback("Process_getFont", Process_getFont);

   registrator.addCallback("Server_init", Server_init);
   registrator.addCallback("Server_listen", Server_listen);
   registrator.addCallback("Server_update", Server_update);
   registrator.addCallback("Server_setActionMap", Server_setActionMap);
   registrator.addCallback("Server_loadWorld", Server_loadWorld);
   registrator.addCallback("Server_sendScriptEvent", Server_sendScriptEvent);

   registrator.addCallback("Client_init", Client_init);
   registrator.addCallback("Client_connect", Client_connect);
   registrator.addCallback("Client_update", Client_update);
   registrator.addCallback("Client_nativeRender", Client_nativeRender);
   registrator.addCallback("Client_nativeDisplay", Client_nativeDisplay);
   registrator.addCallback("Client_isActive", Client_isActive);
   registrator.addCallback("Client_setWindow", Client_setWindow);
   registrator.addCallback("Client_setActionMap", Client_setActionMap);
   registrator.addCallback("Client_setKeyMap", Client_setKeyMap);

   registrator.addCallback("ScriptManager_spawnChild", ScriptManager_spawnChild);

   registrator.addCallback("GameWindowFactory_createWindow", GameWindowFactory_createWindow);

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
   registrator.addCallback("Creature_setController", Creature_setController);

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

   registrator.addCallback("InputController_init", InputController_init);
   registrator.addCallback("InputController_setActionMap", InputController_setActionMap);

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
   registrator.addCallback("ActionMap_setProcess", ActionMap_setProcess);

   registrator.addCallback("KeyMap_init", KeyMap_init);
   registrator.addCallback("KeyMap_bind", KeyMap_bind);

   registrator.addCallback("EngineGraphics_init", EngineGraphics_init);
   registrator.addCallback("EngineGraphics_doSetColor", EngineGraphics_doSetColor);
   registrator.addCallback("EngineGraphics_nativeSetFont", EngineGraphics_nativeSetFont);
   registrator.addCallback("EngineGraphics_translate", EngineGraphics_translate);
   registrator.addCallback("EngineGraphics_drawText", EngineGraphics_drawText);
   registrator.addCallback("EngineGraphics_native_fillRect", EngineGraphics_native_fillRect);
   registrator.addCallback("EngineGraphics_native_drawRect", EngineGraphics_native_drawRect);
   registrator.addCallback("EngineGraphics_native_drawRoundedRect", EngineGraphics_native_drawRoundedRect);

   registrator.addCallback("Font_render", Font_render);
   registrator.addCallback("Font_getBaseLine", Font_getBaseLine);
   registrator.addCallback("Font_native_textWidth", Font_native_textWidth);
   registrator.addCallback("Font_native_textHeight", Font_native_textHeight);

   registrator.registerCallbacks(manager);
}
