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
#include "core/streams/datastream.h"
#include "core/streams/bufferedstream.h"
#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/defines.h"

#include "net/netstream.h"

#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualarray.h"
#include "script/vm/virtualstackaccessor.h"

#include "script/scriptobject.h"

#include "physics/inputforcegenerator.h"
#include "physics/box2d/box2dbody.h"
#include "physics/box2d/box2dsimulator.h"
#include "physics/box2d/box2drevolutejoint.h"
#include "physics/box2d/box2dropejoint.h"

#include "ui/graphics.h"
#include "ui/font.h"

#include "resource/resourcemanager.h"

#include "window/gamewindowfactory.h"

#include "world/world.h"
#include "engine/content/contentmanager.h"

#include "actionmap.h"
#include "entity.h"
#include "keymap.h"
#include "client.h"
#include "player.h"
#include "server.h"
#include "inputcontroller.h"
#include "aicontroller.h"
#include "texture.h"

#define GET_THIS(type, variable)                   type& variable = *static_cast<type*>(accessor.getThis().getNativeObject())
#define DESTRUCT_THIS(type)                        delete static_cast<type*>(accessor.getThis().useNativeObject());

#define RETURN(type, pointer)                      accessor.setResult(*machine.instantiateNative(#type, pointer, false))
#define RETURN_OWNED(type, pointer)                accessor.setResult(*machine.instantiateNative(#type, pointer, true))

#define RETURN_CLASS(class, type, pointer)         accessor.setResult(*machine.instantiateNative(class, pointer, false))
#define RETURN_CLASS_OWNED(class, type, pointer)   accessor.setResult(*machine.instantiateNative(class, pointer, true))

void Process_getFont(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& name = accessor.getString(1);
   int size = accessor.getInt(2);

   FontPtr* pfont = new FontPtr(ResourceManager::getInstance().getFont(name, size));
   RETURN_CLASS_OWNED("engine.ui.Font", FontPtr, pfont);
}

void Process_getTexture(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& name = accessor.getString(1);

   TexturePtr* ptexture = new TexturePtr(ResourceManager::getInstance().getTexture(name));
   RETURN_CLASS_OWNED("engine.core.Texture", TexturePtr, ptexture);
}

void Process_getContentManager(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Process, process);

   RETURN_CLASS("engine.game.ContentManager", ContentManager, &process.getContentManager());
}

void Process_native_setWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Process, process);

   World* pworld = static_cast<World*>(accessor.getObject(1).useNativeObject());
   process.setWorld(pworld);
}

void Process_swapLeakDetection(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Process, process);

   process.swapLeakDetection();
}

void Server_listen(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Server, server);

   int port = accessor.getInt(1);

   accessor.setResult(server.listen(port));
}

void Server_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Server, server);

   ActionMap* pmap = (ActionMap*) accessor.getObject(1).useNativeObject();

   server.setActionMap(pmap);
}

void Server_sendScriptEvent(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Server, server);

   int client = accessor.getInt(1);
   NetStream* pstream = (NetStream*) accessor.getObject(2).getNativeObject();

   server.sendScriptEvent(client, *pstream);
}

void Client_connect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   String ip = accessor.getString(1);
   int port = accessor.getInt(2);

   accessor.setResult(client.connect(ip, port));
}

void Client_nativeRender(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   float delta = accessor.getReal(1);

   client.render(delta);
}

void Client_nativeDisplay(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   client.display();
}

void Client_isActive(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   accessor.setResult(client.isActive());
}

void Client_getWindowFactory(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   RETURN_CLASS("system.GameWindowFactory", GameWindowFactory, &client.getWindowFactory());
}

void Client_native_setWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   GameWindow* pwindow = (GameWindow*) accessor.getObject(1).useNativeObject();

   client.setWindow(pwindow);
}

void Client_getPlayer(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   Player& player = client.getPlayer();
   RETURN_CLASS("engine.game.Player", Player, &player);
}

void Client_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   ActionMap* pmap = (ActionMap*) accessor.getObject(1).useNativeObject();

   client.setActionMap(pmap);
}

void Client_setKeyMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   KeyMap* pmap = (KeyMap*) accessor.getObject(1).useNativeObject();

   client.setKeyMap(pmap);
}

void ContentManager_native_loadEntity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ContentManager, contentmanager);

   const String& filename = accessor.getString(1);

   Entity* presult = contentmanager.loadEntity(filename);
   RETURN_CLASS_OWNED(presult->getClassName(), Entity, presult);
}

void ContentManager_load(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ContentManager, contentmanager);

   const String& filename = accessor.getString(1);

   World* presult = contentmanager.load(filename);
   RETURN_CLASS_OWNED("engine.game.World", World, presult);
}

void GameWindowFactory_createWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(GameWindowFactory, factory);

   RETURN_CLASS_OWNED("system.GameWindow", GameWindow, factory.createWindow());
}

void GameWindow_create(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(GameWindow, window);

   const String& title = accessor.getString(1);
   int width = accessor.getInt(2);
   int height = accessor.getInt(3);
   int bitdepth = accessor.getInt(4);
   bool fullscreen = accessor.getBoolean(5);

   accessor.setResult(window.create(title, width, height, bitdepth, fullscreen));
}

void BufferedStream_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   BufferedStream* pstream = new BufferedStream();
   machine.registerNative(thisobject, pstream);
   thisobject.setOwner(true);
}

void BufferedStream_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(BufferedStream);
}

void NetStream_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   BufferedStream* pbufferedstream = (BufferedStream*) accessor.getObject(1).getNativeObject();

   NetStream* pstream = new NetStream(*pbufferedstream);
   machine.registerNative(thisobject, pstream);
   thisobject.setOwner(true);
}

void NetStream_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(NetStream);
}

void NetStream_writeInt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(NetStream, stream);

   int value = accessor.getInt(1);

   stream.writeInt(value);
}

void NetStream_readInt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(NetStream, stream);

   int value;
   stream.readInt(value);

   accessor.setResult(value);
}

void NetStream_clear(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(NetStream, stream);

   stream.clear();
}

void Entity_getId(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Entity, entity);

   accessor.setResult((int)entity.getId());
}

void Actor_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Actor* pactor = new Actor();
   machine.registerNative(thisobject, pactor);
   thisobject.setOwner(true);
}

void Actor_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(Actor);
}

void Actor_getPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   RETURN_CLASS("engine.core.Vector2D", Vector, const_cast<Vector*>(&actor.getPosition()));
}

void Actor_setPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Vector* ppos = (Vector*) accessor.getObject(1).getNativeObject();

   actor.setPosition(*ppos);
}

void Actor_getVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   RETURN_CLASS("engine.core.Vector2D", Vector, const_cast<Vector*>(&actor.getVelocity()));
}

void Actor_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Vector* ppos = (Vector*) accessor.getObject(1).getNativeObject();

   actor.setVelocity(*ppos);
}

void Actor_setName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   const String& name = accessor.getString(1);

   actor.setName(name);
}

void Actor_setAnimation(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   int index = accessor.getInt(1);

   actor.setAnimation(index);
}

void Actor_direction(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   accessor.setResult(actor.direction());
}

void Actor_flip(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   actor.flip();
}

void Actor_getBody(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   RETURN_CLASS("box2d.Box2DBody", Body, &actor.getBody());
}

void Actor_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Controller* pcontroller = (Controller*) accessor.getObject(1).useNativeObject();

   actor.setController(pcontroller);
}

void Actor_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Actor* pchild = (Actor*) accessor.getObject(1).getNativeObject();

   actor.add(*pchild);
}

void Actor_hasLineOfSight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Actor* pto = (Actor*) accessor.getObject(1).getNativeObject();

   accessor.setResult(actor.hasLineOfSight(*pto));
}

void Player_getClientId(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Player, player);

   accessor.setResult(player.getClientId());
}

void Player_native_getController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Player, player);

   RETURN(Actor, &player.getController());
}

void Player_native_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Player, player);

   Actor& entity = *static_cast<Actor*>(accessor.getObject(1).getNativeObject());

   player.setController(entity);
}

void Vector2D_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Vector* pvector = new Vector();
   machine.registerNative(thisobject, pvector);
   thisobject.setOwner(true);
}

void Vector2D_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(Vector);
}

void Vector2D_getX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Vector, vector);

   accessor.setResult(vector.x);
}

void Vector2D_setX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Vector, vector);

   float x = accessor.getReal(1);

   vector.x = x;
}

void Vector2D_getY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Vector, vector);

   accessor.setResult(vector.y);
}

void Vector2D_setY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Vector, vector);

   float y = accessor.getReal(1);

   vector.y = y;
}

void Vector2D_distance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Vector, vector);

   Vector* pto = static_cast<Vector*>(accessor.getObject(1).getNativeObject());

   accessor.setResult(vector.distance(*pto));
}

void World_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   World* pworld = new World();
   machine.registerNative(thisobject, pworld);
   thisobject.setOwner(true);
}

void World_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(World);
}

void World_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   accessor.setResult(world.getName());
}

void World_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   Actor* pentity = static_cast<Actor*>(accessor.getObject(1).useNativeObject());

   world.addEntity(pentity);
}

void World_setObjectLayer(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   int layer = accessor.getInt(1);

   world.setObjectLayer(layer);
}

void World_setFollowMode(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   int mode = accessor.getInt(1);

   world.setScrollMode(mode);
}

void World_getFollowActor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   RETURN_CLASS("engine.game.Actor", Actor, world.getFollowObject());
}

void World_setFollowActor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   Actor* pentity = static_cast<Actor*>(accessor.getObject(1).getNativeObject());

   world.setFollowObject(pentity);
}

void World_getSimulator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   RETURN_CLASS("box2d.Box2DSimulator", Box2DSimulator, &(Box2DSimulator&)world.getSimulator());
}

void World_setFollowBorders(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   int left = accessor.getInt(1);
   int right = accessor.getInt(2);
   int top = accessor.getInt(3);
   int bottom = accessor.getInt(4);

   world.setFollowBorders(left, right, top, bottom);
}

void World_setFollowBorderWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   int width = accessor.getInt(1);
   
   world.setFollowBorderWidth(width);
}

void World_findEntity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   int controllerid = accessor.getInt(1);
   Entity* pentity = world.findEntity(Id(controllerid));

   RETURN_CLASS("engine.game.Actor", Actor, static_cast<Actor*>(pentity));
}

void InputForceGenerator_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   InputForceGenerator* pgenerator = new InputForceGenerator();
   machine.registerNative(thisobject, pgenerator);
   thisobject.setOwner(true);
}

void InputForceGenerator_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(InputForceGenerator);
}

void InputForceGenerator_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   Vector* pvel = (Vector*) accessor.getObject(1).getNativeObject();

   generator.setVelocity(*pvel);
}

void InputForceGenerator_setImpulse(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   Vector* pvel = (Vector*) accessor.getObject(1).getNativeObject();

   generator.setImpulse(*pvel);
}

void InputController_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   InputController* pcontroller = new InputController();
   machine.registerNative(thisobject, pcontroller);
   thisobject.setOwner(true);
}

void InputController_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(InputController);
}

void InputController_native_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(InputController, controller);

   ActionMap* pactionmap = (ActionMap*) accessor.getObject(1).getNativeObject();

   controller.setActionMap(pactionmap);
}

void AIController_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Process* pprocess = (Process*) accessor.getObject(1).getNativeObject();

   AIController* pcontroller = new AIController(*pprocess);
   pcontroller->setThis(thisobject);
   machine.registerNative(thisobject, pcontroller);
   thisobject.setOwner(true);
}

void AIController_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(AIController);
}

void Box2DSimulator_lineOfSight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DSimulator, simulator);

   const Actor& from = *(Actor*) accessor.getObject(1).getNativeObject();
   const Actor& to = *(Actor*) accessor.getObject(2).getNativeObject();

   accessor.setResult(simulator.lineOfSight(from.getBody(), to.getBody()));
}

void Box2DSimulator_createRevoluteJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DSimulator, simulator);

   Box2DRevoluteJointDefinition* pjointdef = (Box2DRevoluteJointDefinition*) accessor.getObject(1).getNativeObject();

   simulator.createRevoluteJoint(*pjointdef);
}

void Box2DSimulator_createRopeJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DSimulator, simulator);

   Box2DRopeJointDefinition* pjointdef = (Box2DRopeJointDefinition*) accessor.getObject(1).getNativeObject();

   simulator.createRopeJoint(*pjointdef);
}

void Box2DBody_addForceGenerator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DBody, body);

   InputForceGenerator* pgenerator = (InputForceGenerator*) accessor.getObject(1).useNativeObject();

   body.addForceGenerator(pgenerator);
}

void Box2DBody_generateSensors(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DBody, body);

   body.generateSensors();
}

void Box2DRevoluteJointDefinition_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Box2DRevoluteJointDefinition* pjointdef = new Box2DRevoluteJointDefinition();
   machine.registerNative(thisobject, pjointdef);
   thisobject.setOwner(true);
}

void Box2DRevoluteJointDefinition_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(Box2DRevoluteJointDefinition);
}

void Box2DRevoluteJointDefinition_getLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRevoluteJointDefinition, joint);

   RETURN_CLASS("box2d.Box2DBody", Box2DBody, joint.pleft);
}

void Box2DRevoluteJointDefinition_setLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRevoluteJointDefinition, joint);

   VirtualObject& left = accessor.getObject(1);

   joint.pleft = (Box2DBody*)left.getNativeObject();
}

void Box2DRevoluteJointDefinition_getRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRevoluteJointDefinition, joint);

   RETURN_CLASS("box2d.Box2DBody", Box2DBody, joint.pright);
}

void Box2DRevoluteJointDefinition_setRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRevoluteJointDefinition, joint);

   VirtualObject& right = accessor.getObject(1);

   joint.pright = (Box2DBody*)right.getNativeObject();
}

void Box2DRevoluteJointDefinition_getAnchor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRevoluteJointDefinition, joint);

   RETURN_CLASS("engine.core.Vector2D", Vector, &joint.anchor);
}

void Box2DRevoluteJointDefinition_setAnchor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRevoluteJointDefinition, joint);

   joint.anchor = *(Vector*)accessor.getObject(1).getNativeObject();
}

void Box2DRopeJointDefinition_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Box2DRopeJointDefinition* pjointdef = new Box2DRopeJointDefinition();
   machine.registerNative(thisobject, pjointdef);
   thisobject.setOwner(true);
}

void Box2DRopeJointDefinition_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(Box2DRopeJointDefinition);
}

void Box2DRopeJointDefinition_getLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   RETURN_CLASS("box2d.Box2DBody", Box2DBody, joint.pleft);
}

void Box2DRopeJointDefinition_setLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   VirtualObject& left = accessor.getObject(1);

   joint.pleft = (Box2DBody*)left.getNativeObject();
}

void Box2DRopeJointDefinition_getRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   RETURN_CLASS("box2d.Box2DBody", Box2DBody, joint.pright);
}

void Box2DRopeJointDefinition_setRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   VirtualObject& right = accessor.getObject(1);

   joint.pright = (Box2DBody*)right.getNativeObject();
}

void Box2DRopeJointDefinition_getLocalAnchorLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   RETURN_CLASS("engine.core.Vector2D", Vector, &joint.anchorLeft);
}

void Box2DRopeJointDefinition_setLocalAnchorLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   joint.anchorLeft = *(Vector*)accessor.getObject(1).getNativeObject();
}

void Box2DRopeJointDefinition_getLocalAnchorRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   RETURN_CLASS("engine.core.Vector2D", Vector, &joint.anchorRight);
}

void Box2DRopeJointDefinition_setLocalAnchorRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   joint.anchorRight = *(Vector*)accessor.getObject(1).getNativeObject();
}

void ActionMap_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   ActionMap* pmap = new ActionMap();
   machine.registerNative(thisobject, pmap);
   thisobject.setOwner(true);
}

void ActionMap_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(ActionMap);
}

void ActionMap_getProcess(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ActionMap, map);
   
   RETURN_CLASS("engine.Process", Process, &map.getProcess());
}

void ActionMap_setProcess(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ActionMap, map);

   Process* pprocess = (Process*) accessor.getObject(1).getNativeObject();

   map.setProcess(*pprocess);
}

void ActionMap_bind(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ActionMap, map);

   int action = accessor.getInt(1);
   const String& fnc = accessor.getString(2);

   map.bind(action, fnc);
}

void KeyMap_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   KeyMap* pmap = new KeyMap();
   machine.registerNative(thisobject, pmap);
   thisobject.setOwner(true);
}

void KeyMap_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(KeyMap);
}

void KeyMap_bind(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(KeyMap, map);

   int key = accessor.getInt(1);
   int action = accessor.getInt(2);

   map.bind(key, action);
}

void EngineGraphics_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Graphics* pgraphics = new Graphics();
   machine.registerNative(thisobject, pgraphics);
   thisobject.setOwner(true);
}

void EngineGraphics_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(Graphics)
}

void EngineGraphics_doSetColor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   float red = accessor.getReal(1);
   float green = accessor.getReal(2);
   float blue = accessor.getReal(3);
   float alpha = accessor.getReal(4);

   graphics.setColor(red, green, blue, alpha);
}

void EngineGraphics_nativeSetFont(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   FontPtr* pfont = (FontPtr*)accessor.getObject(1).getNativeObject();

   graphics.setFont(*(pfont->ptr()));
}

void EngineGraphics_translate(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);

   graphics.translate(x, y);
}

void EngineGraphics_drawText(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   const String& text = accessor.getString(3);

   graphics.drawText(x, y, text);
}

void EngineGraphics_native_drawTexture(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   TexturePtr* ptexture = (TexturePtr*) accessor.getObject(1).getNativeObject();
   int x      = accessor.getInt(2);
   int y      = accessor.getInt(3);
   int width  = accessor.getInt(4);
   int height = accessor.getInt(5);

   graphics.drawTexture(**ptexture, x, y, width, height);
}

void EngineGraphics_native_fillRect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   graphics.fillRect(x, y, width, height);
}

void EngineGraphics_native_drawRect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   graphics.drawRect(x, y, width, height);
}

void EngineGraphics_native_drawRoundedRect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   float x = accessor.getInt(1);
   float y = accessor.getInt(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   graphics.drawRoundedRect(x, y, width, height);
}

void Font_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(FontPtr);
}

void Font_render(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FontPtr, font);

   const String& text = accessor.getString(1);

   font->render(text);
}

void Font_native_textWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FontPtr, font);

   const String& text = accessor.getString(1);

   accessor.setResult(font->getTextWidth(text));
}

void Font_native_textHeight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FontPtr, font);

   const String& text = accessor.getString(1);

   accessor.setResult(font->getTextHeight(text));
}

void Font_getBaseLine(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FontPtr, font);

   accessor.setResult(font->getBaseLine());
}

void Texture_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(TexturePtr);
}

void Texture_getWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getWidth());
}

void Texture_getHeight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getHeight());
}

void FileSystem_getInstance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   RETURN_CLASS("engine.io.FileSystem", FileSystem, &FileSystem::getInstance());
}

void FileSystem_native_open(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FileSystem, fs);

   const String& name = accessor.getString(1);
   int modus = accessor.getInt(2);

   RETURN_CLASS_OWNED("engine.io.File", File, fs.open(name, modus));
}

void File_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(File);
}

void File_length(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(File, file);

   accessor.setResult(file.size());
}

void File_readText(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(File, file);

   int length = file.size();
   char* pbuffer = new char[length];
   length = file.read(pbuffer, length);

   VirtualArray* parray = machine.instantiateArray();
   parray->addLevel(length);
   
   for ( int index = 0; index < length; index++ )
   {
      (*parray)[index].setChar(pbuffer[index]);
   }

   delete[] pbuffer;

   accessor.setResult(*parray);
}

// - Registration

void script_engine_register(ScriptManager& manager)
{
   ScriptRegistrator registrator;

   registrator.addCallback("Process_getFont", Process_getFont);
   registrator.addCallback("Process_getTexture", Process_getTexture);
   registrator.addCallback("Process_getContentManager", Process_getContentManager);
   registrator.addCallback("Process_native_setWorld", Process_native_setWorld);
   registrator.addCallback("Process_swapLeakDetection", Process_swapLeakDetection);

   registrator.addCallback("Server_listen", Server_listen);
   registrator.addCallback("Server_setActionMap", Server_setActionMap);
   registrator.addCallback("Server_sendScriptEvent", Server_sendScriptEvent);

   registrator.addCallback("Client_connect", Client_connect);
   registrator.addCallback("Client_nativeRender", Client_nativeRender);
   registrator.addCallback("Client_nativeDisplay", Client_nativeDisplay);
   registrator.addCallback("Client_isActive", Client_isActive);
   registrator.addCallback("Client_getWindowFactory", Client_getWindowFactory);
   registrator.addCallback("Client_native_setWindow", Client_native_setWindow);
   registrator.addCallback("Client_setActionMap", Client_setActionMap);
   registrator.addCallback("Client_setKeyMap", Client_setKeyMap);
   registrator.addCallback("Client_getPlayer", Client_getPlayer);

   registrator.addCallback("ContentManager_native_loadEntity", ContentManager_native_loadEntity);
   registrator.addCallback("ContentManager_load", ContentManager_load);

   registrator.addCallback("GameWindowFactory_createWindow", GameWindowFactory_createWindow);

   registrator.addCallback("GameWindow_create", GameWindow_create);

   registrator.addCallback("BufferedStream_init", BufferedStream_init);
   registrator.addCallback("BufferedStream_destruct", BufferedStream_destruct);

   registrator.addCallback("NetStream_init", NetStream_init);
   registrator.addCallback("NetStream_destruct", NetStream_destruct);
   registrator.addCallback("NetStream_writeInt", NetStream_writeInt);
   registrator.addCallback("NetStream_readInt", NetStream_readInt);
   registrator.addCallback("NetStream_clear", NetStream_clear);

   registrator.addCallback("Entity_getId", Entity_getId);

   registrator.addCallback("Actor_init", Actor_init);
   registrator.addCallback("Actor_destruct", Actor_destruct);
   registrator.addCallback("Actor_getPosition", Actor_getPosition);
   registrator.addCallback("Actor_setPosition", Actor_setPosition);
   registrator.addCallback("Actor_getVelocity", Actor_getVelocity);
   registrator.addCallback("Actor_setVelocity", Actor_setVelocity);
   registrator.addCallback("Actor_setName", Actor_setName);
   registrator.addCallback("Actor_setAnimation", Actor_setAnimation);
   registrator.addCallback("Actor_getBody", Actor_getBody);
   registrator.addCallback("Actor_direction", Actor_direction);
   registrator.addCallback("Actor_flip", Actor_flip);
   registrator.addCallback("Actor_setController", Actor_setController);
   registrator.addCallback("Actor_add", Actor_add);
   registrator.addCallback("Actor_hasLineOfSight", Actor_hasLineOfSight);

   registrator.addCallback("Player_getClientId", Player_getClientId);
   registrator.addCallback("Player_native_getController", Player_native_getController);
   registrator.addCallback("Player_native_setController", Player_native_setController);

   registrator.addCallback("Vector2D_init", Vector2D_init);
   registrator.addCallback("Vector2D_destruct", Vector2D_destruct);
   registrator.addCallback("Vector2D_getX", Vector2D_getX);
   registrator.addCallback("Vector2D_setX", Vector2D_setX);
   registrator.addCallback("Vector2D_getY", Vector2D_getY);
   registrator.addCallback("Vector2D_setY", Vector2D_setY);
   registrator.addCallback("Vector2D_distance", Vector2D_distance);

   registrator.addCallback("World_init", World_init);
   registrator.addCallback("World_destruct", World_destruct);
   registrator.addCallback("World_getName", World_getName);
   registrator.addCallback("World_add", World_add);
   registrator.addCallback("World_setObjectLayer", World_setObjectLayer);
   registrator.addCallback("World_setFollowMode", World_setFollowMode);
   registrator.addCallback("World_getFollowActor", World_getFollowActor);
   registrator.addCallback("World_setFollowActor", World_setFollowActor);
   registrator.addCallback("World_setFollowBorders", World_setFollowBorders);
   registrator.addCallback("World_setFollowBorderWidth", World_setFollowBorderWidth);
   registrator.addCallback("World_getSimulator", World_getSimulator);
   registrator.addCallback("World_findEntity", World_findEntity);

   registrator.addCallback("InputForceGenerator_init", InputForceGenerator_init);
   registrator.addCallback("InputForceGenerator_destruct", InputForceGenerator_destruct);
   registrator.addCallback("InputForceGenerator_setVelocity", InputForceGenerator_setVelocity);
   registrator.addCallback("InputForceGenerator_setImpulse", InputForceGenerator_setImpulse);

   registrator.addCallback("InputController_init", InputController_init);
   registrator.addCallback("InputController_destruct", InputController_destruct);
   registrator.addCallback("InputController_native_setActionMap", InputController_native_setActionMap);

   registrator.addCallback("AIController_init", AIController_init);
   registrator.addCallback("AIController_destruct", AIController_destruct);

   registrator.addCallback("Box2DSimulator_lineOfSight", Box2DSimulator_lineOfSight);
   registrator.addCallback("Box2DSimulator_createRevoluteJoint", Box2DSimulator_createRevoluteJoint);
   registrator.addCallback("Box2DSimulator_createRopeJoint", Box2DSimulator_createRopeJoint);

   registrator.addCallback("Box2DBody_addForceGenerator", Box2DBody_addForceGenerator);
   registrator.addCallback("Box2DBody_generateSensors", Box2DBody_generateSensors);

   registrator.addCallback("Box2DRevoluteJointDefinition_init", Box2DRevoluteJointDefinition_init);
   registrator.addCallback("Box2DRevoluteJointDefinition_destruct", Box2DRevoluteJointDefinition_destruct);
   registrator.addCallback("Box2DRevoluteJointDefinition_getLeft", Box2DRevoluteJointDefinition_getLeft);
   registrator.addCallback("Box2DRevoluteJointDefinition_setLeft", Box2DRevoluteJointDefinition_setLeft);
   registrator.addCallback("Box2DRevoluteJointDefinition_getRight", Box2DRevoluteJointDefinition_getRight);
   registrator.addCallback("Box2DRevoluteJointDefinition_setRight", Box2DRevoluteJointDefinition_setRight);
   registrator.addCallback("Box2DRevoluteJointDefinition_getAnchor", Box2DRevoluteJointDefinition_getAnchor);
   registrator.addCallback("Box2DRevoluteJointDefinition_setAnchor", Box2DRevoluteJointDefinition_setAnchor);

   registrator.addCallback("Box2DRopeJointDefinition_init", Box2DRopeJointDefinition_init);
   registrator.addCallback("Box2DRopeJointDefinition_destruct", Box2DRopeJointDefinition_destruct);
   registrator.addCallback("Box2DRopeJointDefinition_getLeft", Box2DRopeJointDefinition_getLeft);
   registrator.addCallback("Box2DRopeJointDefinition_setLeft", Box2DRopeJointDefinition_setLeft);
   registrator.addCallback("Box2DRopeJointDefinition_getRight", Box2DRopeJointDefinition_getRight);
   registrator.addCallback("Box2DRopeJointDefinition_setRight", Box2DRopeJointDefinition_setRight);
   registrator.addCallback("Box2DRopeJointDefinition_getLocalAnchorLeft", Box2DRopeJointDefinition_getLocalAnchorLeft);
   registrator.addCallback("Box2DRopeJointDefinition_setLocalAnchorLeft", Box2DRopeJointDefinition_setLocalAnchorLeft);
   registrator.addCallback("Box2DRopeJointDefinition_getLocalAnchorRight", Box2DRopeJointDefinition_getLocalAnchorRight);
   registrator.addCallback("Box2DRopeJointDefinition_setLocalAnchorRight", Box2DRopeJointDefinition_setLocalAnchorRight);

   registrator.addCallback("ActionMap_init", ActionMap_init);
   registrator.addCallback("ActionMap_destruct", ActionMap_destruct);
   registrator.addCallback("ActionMap_getProcess", ActionMap_getProcess);
   registrator.addCallback("ActionMap_setProcess", ActionMap_setProcess);
   registrator.addCallback("ActionMap_bind", ActionMap_bind);

   registrator.addCallback("KeyMap_init", KeyMap_init);
   registrator.addCallback("KeyMap_destruct", KeyMap_destruct);
   registrator.addCallback("KeyMap_bind", KeyMap_bind);

   registrator.addCallback("EngineGraphics_init", EngineGraphics_init);
   registrator.addCallback("EngineGraphics_destruct", EngineGraphics_destruct);
   registrator.addCallback("EngineGraphics_doSetColor", EngineGraphics_doSetColor);
   registrator.addCallback("EngineGraphics_nativeSetFont", EngineGraphics_nativeSetFont);
   registrator.addCallback("EngineGraphics_translate", EngineGraphics_translate);
   registrator.addCallback("EngineGraphics_drawText", EngineGraphics_drawText);
   registrator.addCallback("EngineGraphics_native_fillRect", EngineGraphics_native_fillRect);
   registrator.addCallback("EngineGraphics_native_drawRect", EngineGraphics_native_drawRect);
   registrator.addCallback("EngineGraphics_native_drawRoundedRect", EngineGraphics_native_drawRoundedRect);
   registrator.addCallback("EngineGraphics_native_drawTexture", EngineGraphics_native_drawTexture);

   registrator.addCallback("Font_destruct", Font_destruct);
   registrator.addCallback("Font_render", Font_render);
   registrator.addCallback("Font_getBaseLine", Font_getBaseLine);
   registrator.addCallback("Font_native_textWidth", Font_native_textWidth);
   registrator.addCallback("Font_native_textHeight", Font_native_textHeight);

   registrator.addCallback("Texture_destruct", Texture_destruct);
   registrator.addCallback("Texture_getWidth", Texture_getWidth);
   registrator.addCallback("Texture_getHeight", Texture_getHeight);

   registrator.addCallback("FileSystem_getInstance", FileSystem_getInstance);
   registrator.addCallback("FileSystem_native_open", FileSystem_native_open);

   registrator.addCallback("File_destruct", File_destruct);
   registrator.addCallback("File_length", File_length);
   registrator.addCallback("File_readText", File_readText);

   registrator.registerCallbacks(manager);
}
