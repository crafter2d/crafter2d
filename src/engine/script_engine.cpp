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
#include "core/graphics/effect.h"
#include "core/graphics/font.h"
#include "core/graphics/texture.h"
#include "core/resource/resourcemanager.h"
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

#include "window/gamewindowfactory.h"

#include "world/layer.h"
#include "world/world.h"
#include "engine/content/contentmanager.h"
#include "engine/components/querybodycomponentmessage.h"

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

#define RETURN_CLASS(class, pointer)               accessor.setResult(*machine.instantiateNative(class, pointer, false))
#define RETURN_CLASS_OWNED(class, pointer)         accessor.setResult(*machine.instantiateNative(class, pointer, true))

#define RETURN_CLASS_I(scriptable)                 accessor.setResult(machine.instantiateNative(scriptable, false));

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

   RETURN_CLASS("engine.game.ContentManager", &process.getContentManager());
}

void Process_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Server, server);

   ActionMap* pmap = (ActionMap*) accessor.getObject(1).useNativeObject();

   server.setActionMap(pmap);
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

   RETURN_CLASS("system.GameWindowFactory", &client.getWindowFactory());
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
   RETURN_CLASS("engine.game.Player", &player);
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
   RETURN_CLASS_OWNED("engine.core.Texture", ptexture);
}

void ContentManager_loadEntity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ContentManager, contentmanager);

   const String& filename = accessor.getString(1);

   Entity* presult = contentmanager.loadEntity(filename);
   RETURN_CLASS_OWNED(presult->getClassName(), presult);
}

void ContentManager_load(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ContentManager, contentmanager);

   const String& filename = accessor.getString(1);

   World* presult = contentmanager.loadWorld(filename);
   RETURN_CLASS_OWNED("engine.game.World", presult);
}

void GameWindowFactory_createWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(GameWindowFactory, factory);

   RETURN_CLASS_OWNED("system.GameWindow", factory.createWindow());
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

void QueryBodyComponentMessage_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   QueryBodyComponentMessage* pmsg = new QueryBodyComponentMessage();
   machine.registerNative(thisobject, pmsg);
}

void QueryBodyComponentMessage_getBody(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(QueryBodyComponentMessage, msg);

   RETURN_CLASS_I(msg.getBody());
}

void Entity_getId(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Entity, entity);

   accessor.setResult((int)entity.getId());
}

void Entity_sendComponentMessage(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Entity, entity);

   ComponentMessage* pmsg = (ComponentMessage*) accessor.getObject(1).getNativeObject();
   entity.sendComponentMessage(*pmsg);
}

void Actor_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Actor* pactor = new Actor();
   machine.registerNative(thisobject, pactor);
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

void Player_getController(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Player, player);

   RETURN(Actor, &player.getController());
}

void Player_setController(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

   RETURN_CLASS("engine.game.Actor", world.getFollowObject());
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

   RETURN_CLASS_I(world.getSimulator());
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

   RETURN_CLASS("engine.game.Actor", static_cast<Actor*>(pentity));
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

   RETURN_CLASS("engine.game.Layer", world.getLayer(index));
}

void Layer_getEffect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   RETURN_CLASS("engine.game.Effect", const_cast<Graphics::Effect*>(&layer.getEffect()));
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
   GET_THIS(Graphics::Effect, effect);

   const String& name = accessor.getString(1);

   TexturePtr* ptexture = new TexturePtr(effect.resolveTexture(name));
   RETURN_CLASS_OWNED("engine.core.Texture", ptexture);
}

void InputForceGenerator_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   InputForceGenerator* pgenerator = new InputForceGenerator();
   machine.registerNative(thisobject, pgenerator);
}

void InputForceGenerator_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(InputForceGenerator);
}

void InputForceGenerator_getVelocity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   VirtualObject* pobject = machine.instantiate("engine.core.Vector2D");
   pobject->setMember(0, Variant(generator.getVelocity().x));
   pobject->setMember(1, Variant(generator.getVelocity().y));
   
   accessor.setResult(Variant(pobject));
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

void Box2DSimulator_createRevoluteJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
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

   ForceGenerator* pgenerator = (ForceGenerator*) accessor.getObject(1).useNativeObject();

   body.addForceGenerator(pgenerator);
}

void Box2DRopeJointDefinition_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Box2DRopeJointDefinition* pjointdef = new Box2DRopeJointDefinition();
   machine.registerNative(thisobject, pjointdef);
}

void Box2DRopeJointDefinition_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(Box2DRopeJointDefinition);
}

void Box2DRopeJointDefinition_getLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   RETURN_CLASS("box2d.Box2DBody", joint.pleft);
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

   RETURN_CLASS("box2d.Box2DBody", joint.pright);
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
}

void ActionMap_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(ActionMap);
}

void ActionMap_getProcess(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ActionMap, map);
   
   RETURN_CLASS("engine.Process", &map.getProcess());
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
   RETURN_CLASS("engine.io.FileSystem", &FileSystem::getInstance());
}

void FileSystem_open(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FileSystem, fs);

   const String& name = accessor.getString(1);
   int modus = accessor.getInt(2);

   File* pfile = fs.open(name, modus);
   if ( pfile != NULL )
   {
      RETURN_CLASS_OWNED("engine.io.File", pfile);
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

   registrator.addClass("engine.game.Process");
   //registrator.addFunction("getFont", Process_getFont);
   registrator.addFunction("getContentManager()", Process_getContentManager);
   registrator.addFunction("setActionMap(engine.game.ActionMap)", Process_setActionMap);
   registrator.addFunction("setWorld(engine.game.World)", Process_setWorld);
   registrator.addFunction("swapLeakDetection()", Process_swapLeakDetection);

   registrator.addClass("engine.game.Server");
   registrator.addFunction("listen(int)", Server_listen);
   registrator.addFunction("sendScriptEvent(int, engine.net.NetStream)", Server_sendScriptEvent);

   registrator.addClass("engine.game.Client");
   registrator.addFunction("connect(string, int)", Client_connect);
   registrator.addFunction("isActive()", Client_isActive);
   registrator.addFunction("getWindowFactory()", Client_getWindowFactory);
   registrator.addFunction("setWindow(system.GameWindow)", Client_setWindow);
   registrator.addFunction("setKeyMap(engine.game.KeyMap)", Client_setKeyMap);
   registrator.addFunction("getPlayer()", Client_getPlayer);
   registrator.addFunction("getTexture(string)", Client_getTexture);

   registrator.addClass("engine.game.ContentManager");
   registrator.addFunction("loadEntity(string)", ContentManager_loadEntity);
   registrator.addFunction("load(string)", ContentManager_load);

   registrator.addClass("system.GameWindowFactory");
   registrator.addFunction("createWindow()", GameWindowFactory_createWindow);

   registrator.addClass("system.GameWindow");
   registrator.addFunction("create(string, int, int, int, boolean)", GameWindow_create);

   registrator.addClass("engine.streams.BufferedStream");
   registrator.addFunction("BufferedStream()", BufferedStream_init);
   registrator.addFunction("finalize()", BufferedStream_destruct);

   registrator.addClass("engine.net.NetStream");
   registrator.addFunction("NetStream(engine.streams.BufferedStream)", NetStream_init);
   registrator.addFunction("finalize()", NetStream_destruct);
   registrator.addFunction("writeInt(int)", NetStream_writeInt);
   registrator.addFunction("readInt()", NetStream_readInt);
   registrator.addFunction("clear()", NetStream_clear);

   registrator.addClass("engine.game.Entity");
   registrator.addFunction("getId()", Entity_getId);
   registrator.addFunction("sendComponentMessage(engine.game.ComponentMessage)", Entity_sendComponentMessage);

   registrator.addClass("engine.game.QueryBodyComponentMessage");
   registrator.addFunction("QueryBodyComponentMessage()", QueryBodyComponentMessage_init);
   registrator.addFunction("getBody()", QueryBodyComponentMessage_getBody);

   registrator.addClass("engine.game.Actor");
   registrator.addFunction("Actor()", Actor_init);
   registrator.addFunction("finalize()", Actor_destruct);
   registrator.addFunction("getPositionX()", Actor_getPositionX);
   registrator.addFunction("getPositionY()", Actor_getPositionY);
   registrator.addFunction("setPosition(real, real)", Actor_setPosition);
   registrator.addFunction("setName(string)", Actor_setName);
   registrator.addFunction("setAnimation(int)", Actor_setAnimation);
   registrator.addFunction("direction()", Actor_direction);
   registrator.addFunction("flip()", Actor_flip);
   registrator.addFunction("setController(engine.game.Controller)", Actor_setController);
   registrator.addFunction("add(engine.game.Actor)", Actor_add);
   registrator.addFunction("hasLineOfSight(engine.game.Actor)", Actor_hasLineOfSight);

   registrator.addClass("engine.game.Player");
   registrator.addFunction("getClientId()", Player_getClientId);
   registrator.addFunction("getController()", Player_getController);
   registrator.addFunction("setController(engine.game.Actor)", Player_setController);

   registrator.addClass("engine.game.World");
   registrator.addFunction("World()", World_init);
   registrator.addFunction("finalize()", World_destruct);
   registrator.addFunction("getName()", World_getName);
   registrator.addFunction("add(engine.game.Actor)", World_add);
   registrator.addFunction("setObjectLayer(int)", World_setObjectLayer);
   registrator.addFunction("setFollowMode(int)", World_setFollowMode);
   registrator.addFunction("getFollowActor()", World_getFollowActor);
   registrator.addFunction("setFollowActor(engine.game.Actor)", World_setFollowActor);
   registrator.addFunction("setFollowBorders(int, int, int, int)", World_setFollowBorders);
   registrator.addFunction("setFollowBorderWidth(int)", World_setFollowBorderWidth);
   registrator.addFunction("getSimulator()", World_getSimulator);
   registrator.addFunction("findEntity(int)", World_findEntity);
   registrator.addFunction("getLayerCount()", World_getLayerCount);
   registrator.addFunction("getLayer(int)", World_getLayer);

   registrator.addClass("engine.game.Layer");
   registrator.addFunction("getEffect()", Layer_getEffect);
   registrator.addFunction("getWidth()", Layer_getWidth);
   registrator.addFunction("getHeight()", Layer_getHeight);
   registrator.addFunction("getTile(int, int)", Layer_getTile);
   registrator.addFunction("setTile(int, int, int)", Layer_setTile);

   registrator.addClass("engine.game.Effect");
   registrator.addFunction("resolveTexture(string)", Effect_resolveTexture);

   registrator.addClass("engine.game.InputForceGenerator");
   registrator.addFunction("InputForceGenerator()", InputForceGenerator_init);
   registrator.addFunction("finalize()", InputForceGenerator_destruct);
   registrator.addFunction("getVelocity()", InputForceGenerator_getVelocity);
   registrator.addFunction("setVelocity(real, real)", InputForceGenerator_setVelocity);
   registrator.addFunction("setImpulse(real, real)", InputForceGenerator_setImpulse);

   registrator.addClass("engine.game.InputController");
   registrator.addFunction("InputController()", InputController_init);
   registrator.addFunction("finalize()", InputController_destruct);
   registrator.addFunction("setActionMap(engine.game.ActionMap)", InputController_setActionMap);

   registrator.addClass("engine.game.AIController");
   registrator.addFunction("AIController()", AIController_init);
   registrator.addFunction("finalize()", AIController_destruct);

   registrator.addClass("box2d.Box2DSimulator");
   registrator.addFunction("lineOfSight(Actor, Actor)", Box2DSimulator_lineOfSight);
   registrator.addFunction("createRevoluteJoint(box2d.Box2DBody, box2d.Box2DBody, real, real)", Box2DSimulator_createRevoluteJoint);
   registrator.addFunction("createRopeJoint(box2d.Box2DRopeJointDefinition)", Box2DSimulator_createRopeJoint);

   registrator.addClass("box2d.Box2DBody");
   registrator.addFunction("addForceGenerator(engine.game.ForceGenerator)", Box2DBody_addForceGenerator);

   registrator.addClass("box2d.Box2DRopeJointDefinition");
   registrator.addFunction("Box2DRopeJointDefinition", Box2DRopeJointDefinition_init);
   registrator.addFunction("finalize()", Box2DRopeJointDefinition_destruct);
   registrator.addFunction("getLeft()", Box2DRopeJointDefinition_getLeft);
   registrator.addFunction("setLeft(box2d.Box2DBody)", Box2DRopeJointDefinition_setLeft);
   registrator.addFunction("getRight()", Box2DRopeJointDefinition_getRight);
   registrator.addFunction("setRight(box2d.Box2DBody)", Box2DRopeJointDefinition_setRight);
   registrator.addFunction("getLocalAnchorLeftX()", Box2DRopeJointDefinition_getLocalAnchorLeftX);
   registrator.addFunction("getLocalAnchorLeftY()", Box2DRopeJointDefinition_getLocalAnchorLeftY);
   registrator.addFunction("setLocalAnchorLeft(real, real)", Box2DRopeJointDefinition_setLocalAnchorLeft);
   registrator.addFunction("getLocalAnchorRightX()", Box2DRopeJointDefinition_getLocalAnchorRightX);
   registrator.addFunction("getLocalAnchorRightY()", Box2DRopeJointDefinition_getLocalAnchorRightY);
   registrator.addFunction("setLocalAnchorRight(real, real)", Box2DRopeJointDefinition_setLocalAnchorRight);

   registrator.addClass("engine.game.ActionMap");
   registrator.addFunction("ActionMap()", ActionMap_init);
   registrator.addFunction("finalize()", ActionMap_destruct);
   registrator.addFunction("getProcess()", ActionMap_getProcess);
   registrator.addFunction("setProcess(engine.game.Process)", ActionMap_setProcess);
   registrator.addFunction("bind(int, string)", ActionMap_bind);

   registrator.addClass("engine.game.KeyMap");
   registrator.addFunction("KeyMap()", KeyMap_init);
   registrator.addFunction("finalize()", KeyMap_destruct);
   registrator.addFunction("bind(int, int)", KeyMap_bind);

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

   registrator.addClass("engine.ui.Font");
   registrator.addFunction("finalize()", Font_destruct);
   registrator.addFunction("render(string)", Font_render);
   registrator.addFunction("getBaseLine()", Font_getBaseLine);
   registrator.addFunction("getTextWidth(string)", Font_getTextWidth);
   registrator.addFunction("getTextHeight(string)", Font_getTextHeight);

   registrator.addClass("engine.core.Texture");
   registrator.addFunction("finalize()", Texture_destruct);
   registrator.addFunction("getName()", Texture_getName);
   registrator.addFunction("getWidth()", Texture_getWidth);
   registrator.addFunction("getHeight()", Texture_getHeight);
   registrator.addFunction("getSourceWidth()", Texture_getSourceWidth);
   registrator.addFunction("getSourceHeight()", Texture_getSourceHeight);

   registrator.addClass("engine.io.FileSystem");
   registrator.addFunction("getInstance()", FileSystem_getInstance);
   registrator.addFunction("open(string, int)", FileSystem_open);

   registrator.addClass("engine.io.File");
   registrator.addFunction("finalize()", File_destruct);
   registrator.addFunction("length()", File_length);
   registrator.addFunction("readText()", File_readText);

   registrator.registerCallbacks(manager);
}
