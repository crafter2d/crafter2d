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
#include "core/graphics/font.h"
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

#include "resource/resourcemanager.h"

#include "window/gamewindowfactory.h"

#include "world/layer.h"
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

   //FontPtr* pfont = new FontPtr(ResourceManager::getInstance().getFont(name, size));
   //RETURN_CLASS_OWNED("engine.ui.Font", FontPtr, pfont);
}

void Process_getContentManager(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Process, process);

   RETURN_CLASS("engine.game.ContentManager", ContentManager, &process.getContentManager());
}

void Process_setWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

void Client_setWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

void Client_getTexture(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   const String& name = accessor.getString(1);

   TexturePtr* ptexture = new TexturePtr(ResourceManager::getInstance().getTexture(client.getDevice(), name));
   RETURN_CLASS_OWNED("engine.core.Texture", TexturePtr, ptexture);
}

void ContentManager_loadEntity(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

   World* presult = contentmanager.loadWorld(filename);
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

void Actor_getPositionX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   accessor.setResult(actor.getPosition().x);
}

void Actor_getPositionY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   accessor.setResult(actor.getPosition().y);
}

void Actor_setPosition(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   float xpos = accessor.getReal(1);
   float ypos = accessor.getReal(2);

   actor.setPosition(Vector(xpos, ypos));
}

void Actor_getVelocityX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   accessor.setResult(actor.getVelocity().x);
}

void Actor_getVelocityY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   accessor.setResult(actor.getVelocity().y);
}

void Actor_setVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   float xvel = accessor.getReal(1);
   float yvel = accessor.getReal(2);

   actor.setVelocity(Vector(xvel, yvel));
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

void World_getLayerCount(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   accessor.setResult(world.getLayerCount());
}
 
void World_getLayer(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   int index = accessor.getInt(1);

   RETURN_CLASS("engine.game.Layer", Layer, world.getLayer(index));
}

void Layer_getEffect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   RETURN_CLASS("engine.game.Effect", Effect, const_cast<Effect*>(&layer.getEffect()));
}

void Layer_getWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   accessor.setResult(layer.getWidth());
}

void Layer_getHeight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   accessor.setResult(layer.getHeight());
}

void Layer_getTile(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   int x = accessor.getInt(1);
   int y = accessor.getInt(2);

   accessor.setResult(layer.getTile(x, y));
}

void Layer_setTile(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   int x = accessor.getInt(1);
   int y = accessor.getInt(2);
   int t = accessor.getInt(3);

   layer.setTile(x, y, t);
}

void Effect_resolveTexture(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Effect, effect);

   const String& name = accessor.getString(1);

   TexturePtr* ptexture = new TexturePtr(effect.resolveTexture(name));
   RETURN_CLASS_OWNED("engine.core.Texture", TexturePtr, ptexture);
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

   float xvel = accessor.getReal(1);
   float yvel = accessor.getReal(2);

   generator.setVelocity(Vector(xvel, yvel));
}

void InputForceGenerator_setImpulse(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   float ximp = accessor.getReal(1);
   float yimp = accessor.getReal(2);

   generator.setImpulse(Vector(ximp, yimp));
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

void InputController_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

   accessor.setResult(false); //simulator.lineOfSight(from.getBody(), to.getBody()));
}

void Box2DSimulator_native_createRevoluteJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DSimulator, simulator);

   Box2DBody& left = *(Box2DBody*) accessor.getObject(1).getNativeObject();
   Box2DBody& right = *(Box2DBody*) accessor.getObject(2).getNativeObject();
   float anchorx = accessor.getReal(3);
   float anchory = accessor.getReal(4);

   simulator.createRevoluteJoint(left, right, Vector(anchorx, anchory));
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

void Box2DRopeJointDefinition_getLocalAnchorLeftX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   accessor.setResult(joint.anchorLeft.x);
}

void Box2DRopeJointDefinition_getLocalAnchorLeftY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   accessor.setResult(joint.anchorLeft.y);
}

void Box2DRopeJointDefinition_setLocalAnchorLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   float ximp = accessor.getReal(1);
   float yimp = accessor.getReal(2);

   joint.anchorLeft.set(ximp, yimp);
}

void Box2DRopeJointDefinition_getLocalAnchorRightX(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   accessor.setResult(joint.anchorRight.x);
}

void Box2DRopeJointDefinition_getLocalAnchorRightY(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   accessor.setResult(joint.anchorRight.y);
}

void Box2DRopeJointDefinition_setLocalAnchorRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   float ximp = accessor.getReal(1);
   float yimp = accessor.getReal(2);

   joint.anchorRight.set(ximp, yimp);
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

/*
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

void EngineGraphics_native_drawTexturePart(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Graphics, graphics);

   TexturePtr* ptexture = (TexturePtr*) accessor.getObject(1).getNativeObject();
   int x      = accessor.getInt(2);
   int y      = accessor.getInt(3);
   int width  = accessor.getInt(4);
   int height = accessor.getInt(5);
   float tx   = accessor.getReal(6);
   float ty   = accessor.getReal(7);
   float tw   = accessor.getReal(8);
   float th   = accessor.getReal(9);

   graphics.drawTexture(**ptexture, x, y, width, height, tx, ty, tw, th);
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

*/

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

void Font_getTextWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FontPtr, font);

   const String& text = accessor.getString(1);

   accessor.setResult(font->getTextWidth(text));
}

void Font_getTextHeight(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

void Texture_getName(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getName());
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

void Texture_getSourceWidth(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getSourceWidth());
}

void Texture_getSourceHeight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getSourceHeight());
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

   File* pfile = fs.open(name, modus);
   if ( pfile != NULL )
   {
      RETURN_CLASS_OWNED("engine.io.File", File, pfile);
   }
   else
   {
      accessor.setResult(Variant());
   }   
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

   registrator.addClass("Process");
   registrator.addFunction("getFont", Process_getFont);
   registrator.addFunction("getContentManager", Process_getContentManager);
   registrator.addFunction("setWorld", Process_setWorld);
   registrator.addFunction("swapLeakDetection", Process_swapLeakDetection);

   registrator.addClass("Server");
   registrator.addFunction("listen", Server_listen);
   registrator.addFunction("setActionMap", Server_setActionMap);
   registrator.addFunction("sendScriptEvent", Server_sendScriptEvent);

   registrator.addClass("Client");
   registrator.addFunction("connect", Client_connect);
   registrator.addFunction("isActive", Client_isActive);
   registrator.addFunction("getWindowFactory", Client_getWindowFactory);
   registrator.addFunction("setWindow", Client_setWindow);
   registrator.addFunction("setActionMap", Client_setActionMap);
   registrator.addFunction("setKeyMap", Client_setKeyMap);
   registrator.addFunction("getPlayer", Client_getPlayer);
   registrator.addFunction("getTexture", Client_getTexture);

   registrator.addClass("ContentManager");
   registrator.addFunction("loadEntity", ContentManager_loadEntity);
   registrator.addFunction("load", ContentManager_load);

   registrator.addClass("GameWindowFactory");
   registrator.addFunction("createWindow", GameWindowFactory_createWindow);

   registrator.addClass("GameWindow");
   registrator.addFunction("create", GameWindow_create);

   registrator.addClass("BufferedStream");
   registrator.addFunction("init", BufferedStream_init);
   registrator.addFunction("destruct", BufferedStream_destruct);

   registrator.addClass("NetStream");
   registrator.addFunction("init", NetStream_init);
   registrator.addFunction("destruct", NetStream_destruct);
   registrator.addFunction("writeInt", NetStream_writeInt);
   registrator.addFunction("readInt", NetStream_readInt);
   registrator.addFunction("clear", NetStream_clear);

   registrator.addClass("Entity");
   registrator.addFunction("getId", Entity_getId);

   registrator.addClass("Actor");
   registrator.addFunction("init", Actor_init);
   registrator.addFunction("destruct", Actor_destruct);
   registrator.addFunction("getPositionX", Actor_getPositionX);
   registrator.addFunction("getPositionY", Actor_getPositionY);
   registrator.addFunction("setPosition", Actor_setPosition);
   registrator.addFunction("getVelocityX", Actor_getVelocityX);
   registrator.addFunction("getVelocityY", Actor_getVelocityY);
   registrator.addFunction("setVelocity", Actor_setVelocity);
   registrator.addFunction("setName", Actor_setName);
   registrator.addFunction("setAnimation", Actor_setAnimation);
   registrator.addFunction("direction", Actor_direction);
   registrator.addFunction("flip", Actor_flip);
   registrator.addFunction("setController", Actor_setController);
   registrator.addFunction("add", Actor_add);
   registrator.addFunction("hasLineOfSight", Actor_hasLineOfSight);

   registrator.addClass("Player");
   registrator.addFunction("getClientId", Player_getClientId);
   registrator.addFunction("getController", Player_native_getController);
   registrator.addFunction("setController", Player_native_setController);

   registrator.addClass("World");
   registrator.addFunction("init", World_init);
   registrator.addFunction("destruct", World_destruct);
   registrator.addFunction("getName", World_getName);
   registrator.addFunction("add", World_add);
   registrator.addFunction("setObjectLayer", World_setObjectLayer);
   registrator.addFunction("setFollowMode", World_setFollowMode);
   registrator.addFunction("getFollowActor", World_getFollowActor);
   registrator.addFunction("setFollowActor", World_setFollowActor);
   registrator.addFunction("setFollowBorders", World_setFollowBorders);
   registrator.addFunction("setFollowBorderWidth", World_setFollowBorderWidth);
   registrator.addFunction("getSimulator", World_getSimulator);
   registrator.addFunction("findEntity", World_findEntity);
   registrator.addFunction("getLayerCount", World_getLayerCount);
   registrator.addFunction("getLayer", World_getLayer);

   registrator.addClass("Layer");
   registrator.addFunction("getEffect", Layer_getEffect);
   registrator.addFunction("getWidth", Layer_getWidth);
   registrator.addFunction("getHeight", Layer_getHeight);
   registrator.addFunction("getTile", Layer_getTile);
   registrator.addFunction("setTile", Layer_setTile);

   registrator.addClass("Effect");
   registrator.addFunction("resolveTexture", Effect_resolveTexture);

   registrator.addClass("InputForceGenerator");
   registrator.addFunction("init", InputForceGenerator_init);
   registrator.addFunction("destruct", InputForceGenerator_destruct);
   registrator.addFunction("setVelocity", InputForceGenerator_setVelocity);
   registrator.addFunction("setImpulse", InputForceGenerator_setImpulse);

   registrator.addClass("InputController");
   registrator.addFunction("init", InputController_init);
   registrator.addFunction("destruct", InputController_destruct);
   registrator.addFunction("setActionMap", InputController_setActionMap);

   registrator.addClass("AIController");
   registrator.addFunction("init", AIController_init);
   registrator.addFunction("destruct", AIController_destruct);

   registrator.addClass("Box2DSimulator");
   registrator.addFunction("lineOfSight", Box2DSimulator_lineOfSight);
   registrator.addFunction("createRevoluteJoint", Box2DSimulator_native_createRevoluteJoint);
   registrator.addFunction("createRopeJoint", Box2DSimulator_createRopeJoint);

   registrator.addClass("Box2DBody");
   registrator.addFunction("addForceGenerator", Box2DBody_addForceGenerator);
   registrator.addFunction("generateSensors", Box2DBody_generateSensors);

   registrator.addClass("Box2DRopeJointDefinition");
   registrator.addFunction("init", Box2DRopeJointDefinition_init);
   registrator.addFunction("destruct", Box2DRopeJointDefinition_destruct);
   registrator.addFunction("getLeft", Box2DRopeJointDefinition_getLeft);
   registrator.addFunction("setLeft", Box2DRopeJointDefinition_setLeft);
   registrator.addFunction("getRight", Box2DRopeJointDefinition_getRight);
   registrator.addFunction("setRight", Box2DRopeJointDefinition_setRight);
   registrator.addFunction("getLocalAnchorLeftX", Box2DRopeJointDefinition_getLocalAnchorLeftX);
   registrator.addFunction("getLocalAnchorLeftY", Box2DRopeJointDefinition_getLocalAnchorLeftY);
   registrator.addFunction("setLocalAnchorLeft", Box2DRopeJointDefinition_setLocalAnchorLeft);
   registrator.addFunction("getLocalAnchorRightX", Box2DRopeJointDefinition_getLocalAnchorRightX);
   registrator.addFunction("getLocalAnchorRightY", Box2DRopeJointDefinition_getLocalAnchorRightY);
   registrator.addFunction("setLocalAnchorRight", Box2DRopeJointDefinition_setLocalAnchorRight);

   registrator.addClass("ActionMap");
   registrator.addFunction("init", ActionMap_init);
   registrator.addFunction("destruct", ActionMap_destruct);
   registrator.addFunction("getProcess", ActionMap_getProcess);
   registrator.addFunction("setProcess", ActionMap_setProcess);
   registrator.addFunction("bind", ActionMap_bind);

   registrator.addClass("KeyMap");
   registrator.addFunction("init", KeyMap_init);
   registrator.addFunction("destruct", KeyMap_destruct);
   registrator.addFunction("bind", KeyMap_bind);

   /*
   registrator.addFunction("EngineGraphics_init", EngineGraphics_init);
   registrator.addFunction("EngineGraphics_destruct", EngineGraphics_destruct);
   registrator.addFunction("EngineGraphics_doSetColor", EngineGraphics_doSetColor);
   registrator.addFunction("EngineGraphics_nativeSetFont", EngineGraphics_nativeSetFont);
   registrator.addFunction("EngineGraphics_translate", EngineGraphics_translate);
   registrator.addFunction("EngineGraphics_drawText", EngineGraphics_drawText);
   registrator.addFunction("EngineGraphics_native_fillRect", EngineGraphics_native_fillRect);
   registrator.addFunction("EngineGraphics_native_drawRect", EngineGraphics_native_drawRect);
   registrator.addFunction("EngineGraphics_native_drawRoundedRect", EngineGraphics_native_drawRoundedRect);
   registrator.addFunction("EngineGraphics_native_drawTexture", EngineGraphics_native_drawTexture);
   */

   registrator.addClass("Font");
   registrator.addFunction("destruct", Font_destruct);
   registrator.addFunction("render", Font_render);
   registrator.addFunction("getBaseLine", Font_getBaseLine);
   registrator.addFunction("getTextWidth", Font_getTextWidth);
   registrator.addFunction("getTextHeight", Font_getTextHeight);

   registrator.addClass("Texture");
   registrator.addFunction("destruct", Texture_destruct);
   registrator.addFunction("getName", Texture_getName);
   registrator.addFunction("getWidth", Texture_getWidth);
   registrator.addFunction("getHeight", Texture_getHeight);
   registrator.addFunction("getSourceWidth", Texture_getSourceWidth);
   registrator.addFunction("getSourceHeight", Texture_getSourceHeight);

   registrator.addClass("FileSystem");
   registrator.addFunction("getInstance", FileSystem_getInstance);
   registrator.addFunction("open", FileSystem_native_open);

   registrator.addClass("File");
   registrator.addFunction("destruct", File_destruct);
   registrator.addFunction("length", File_length);
   registrator.addFunction("readText", File_readText);

   registrator.registerCallbacks(manager);
}
