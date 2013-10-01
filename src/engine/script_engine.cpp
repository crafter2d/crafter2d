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

#include "engine/window/gamewindow.h"

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
#include "engine/components/animationcomponentmessage.h"

#include "actionmap.h"
#include "entity.h"
#include "keymap.h"
#include "client.h"
#include "player.h"
#include "server.h"
#include "inputcontroller.h"
#include "aicontroller.h"

#define GET_THIS(type, variable)                   type& variable = accessor.getThis().getNativeObject<type>()
#define DESTRUCT_THIS(type)                        delete accessor.getThis().useNativeObject<type>();

#define RETURN(type, pointer)                      accessor.setResult(*machine.instantiateNative(UTEXT(#type), pointer, false))
#define RETURN_OWNED(type, pointer)                accessor.setResult(*machine.instantiateNativeUTEXT((#type), pointer, true))

#define RETURN_CLASS(class, pointer)               accessor.setResult(*machine.instantiateNative(class, pointer, false))
#define RETURN_CLASS_OWNED(class, pointer)         accessor.setResult(*machine.instantiateNative(class, pointer, true))

#define RETURN_CLASS_I(scriptable)                 accessor.setResult(machine.instantiateNative(scriptable, false));

void Process_getFont(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   //const String& name = accessor.getString(1);
   //int size = accessor.getInt(2);

   //FontPtr* pfont = new FontPtr(ResourceManager::getInstance().getFont(name, size));
   //RETURN_CLASS_OWNED("engine.ui.Font"), FontPtr, pfont);
}

void Process_getContentManager(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Process, process);

   RETURN_CLASS(UTEXT("engine.game.ContentManager"), &process.getContentManager());
}

void Process_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Server, server);

   ActionMap* pmap = accessor.getObject(1).useNativeObject<ActionMap>();

   server.setActionMap(pmap);
}

void Process_loadWorld(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Process, process);

   const String& filename = accessor.getString(1);
   World* pworld = process.loadWorld(filename);

   RETURN_CLASS(UTEXT("engine.game.World"), pworld);
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
   NetStream& stream = accessor.getObject(2).getNativeObject<NetStream>();

   server.sendScriptEvent(client, stream);
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

   RETURN_CLASS(UTEXT("system.GameWindowFactory"), &client.getWindowFactory());
}

void Client_setWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   GameWindow* pwindow = accessor.getObject(1).useNativeObject<GameWindow>();

   client.setWindow(pwindow);
}

void Client_getPlayer(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   Player& player = client.getPlayer();
   RETURN_CLASS(UTEXT("engine.game.Player"), &player);
}

void Client_setActionMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   ActionMap* pmap = accessor.getObject(1).useNativeObject<ActionMap>();

   client.setActionMap(pmap);
}

void Client_setKeyMap(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   KeyMap* pmap = accessor.getObject(1).useNativeObject<KeyMap>();

   client.setKeyMap(pmap);
}

void Client_getTexture(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Client, client);

   const String& name = accessor.getString(1);

   TexturePtr* ptexture = new TexturePtr(ResourceManager::getInstance().getTexture(client.getDevice(), name));
   RETURN_CLASS_OWNED(UTEXT("engine.core.Texture"), ptexture);
}

void ContentManager_loadEntity(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ContentManager, contentmanager);

   const String& filename = accessor.getString(1);

   Entity* presult = contentmanager.loadEntity(filename);
   RETURN_CLASS_OWNED(presult->getClassName(), presult);
}

void GameWindowFactory_createWindow(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(GameWindowFactory, factory);

   RETURN_CLASS_OWNED(UTEXT("system.GameWindow"), factory.createWindow());
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

   BufferedStream& bufferedstream = accessor.getObject(1).getNativeObject<BufferedStream>();

   NetStream* pstream = new NetStream(bufferedstream);
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

void QueryBodyComponentMessage_hasBody(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(QueryBodyComponentMessage, msg);

   accessor.setResult(msg.hasBody());
}

void QueryBodyComponentMessage_getBody(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(QueryBodyComponentMessage, msg);

   RETURN_CLASS_I(msg.getBody());
}

void AnimationComponentMessage_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   AnimationComponentMessage* pmsg = new AnimationComponentMessage();
   machine.registerNative(thisobject, pmsg);
}

void AnimationComponentMessage_setAnimation(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(AnimationComponentMessage, msg);

   msg.setAnimation(accessor.getInt(1));
}

void Entity_getId(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Entity, entity);

   accessor.setResult((int)entity.getId());
}

void Entity_sendComponentMessage(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Entity, entity);

   ComponentMessage& msg = accessor.getObject(1).getNativeObject<ComponentMessage>();
   entity.sendComponentMessage(msg);
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

   Controller* pcontroller = accessor.getObject(1).useNativeObject<Controller>();

   actor.setController(pcontroller);
}

void Actor_add(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Actor& child = accessor.getObject(1).getNativeObject<Actor>();

   actor.add(&child);
}

void Actor_hasLineOfSight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Actor, actor);

   Actor& to = accessor.getObject(1).getNativeObject<Actor>();

   accessor.setResult(actor.hasLineOfSight(to));
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

   Actor& entity = accessor.getObject(1).getNativeObject<Actor>();

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

   Actor* pentity = accessor.getObject(1).useNativeObject<Actor>();

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

   RETURN_CLASS(UTEXT("engine.game.Actor"), world.getFollowObject());
}

void World_setFollowActor(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(World, world);

   Actor& entity = accessor.getObject(1).getNativeObject<Actor>();

   world.setFollowObject(entity);
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

   RETURN_CLASS(UTEXT("engine.game.Actor"), pentity);
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

   RETURN_CLASS(UTEXT("engine.game.Layer"), world.getLayer(index));
}

void Layer_getEffect(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   RETURN_CLASS(UTEXT("engine.game.Effect"), const_cast<Graphics::Effect*>(&layer.getEffect()));
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

   int level = accessor.getInt(1);
   int x = accessor.getInt(2);
   int y = accessor.getInt(3);

   accessor.setResult(layer.getTile((LayerLevel)level, x, y));
}

void Layer_setTile(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Layer, layer);

   int level = accessor.getInt(1);
   int x = accessor.getInt(2);
   int y = accessor.getInt(3);
   int t = accessor.getInt(4);

   layer.setTile((LayerLevel)level, x, y, t);
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

   VirtualObject* pobject = machine.instantiate(UTEXT("engine.core.Vector2D"));
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

   ActionMap& actionmap = accessor.getObject(1).getNativeObject<ActionMap>();

   controller.setActionMap(actionmap);
}

void AIController_init(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();

   Process& process = accessor.getObject(1).getNativeObject<Process>();

   AIController* pcontroller = new AIController(process);
   pcontroller->setThis(thisobject);
   machine.registerNative(thisobject, pcontroller);
}

void AIController_destruct(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   DESTRUCT_THIS(AIController);
}

void Box2DSimulator_lineOfSight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   //GET_THIS(Box2DSimulator, simulator);

   //const Actor& from = accessor.getObject(1).getNativeObject<Actor>();
   //const Actor& to = accessor.getObject(2).getNativeObject<Actor>();

   accessor.setResult(false); //simulator.lineOfSight(from.getBody(), to.getBody()));
}

void Box2DSimulator_createRevoluteJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   /*
   GET_THIS(Box2DSimulator, simulator);

   Box2DBody& left = accessor.getObject(1).getNativeObject<Box2DBody>();
   Box2DBody& right = accessor.getObject(2).getNativeObject<Box2DBody>();
   float anchorx = accessor.getReal(3);
   float anchory = accessor.getReal(4);

   simulator.createRevoluteJoint(left, right, Vector(anchorx, anchory));
   */
}

void Box2DSimulator_createRopeJoint(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   /*
   GET_THIS(Box2DSimulator, simulator);

   Box2DRopeJointDefinition& jointdef = accessor.getObject(1).getNativeObject<Box2DRopeJointDefinition>();

   simulator.createRopeJoint(jointdef);
   */
}

void Box2DBody_addForceGenerator(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DBody, body);

   ForceGenerator* pgenerator = accessor.getObject(1).useNativeObject<ForceGenerator>();

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

   RETURN_CLASS(UTEXT("box2d.Box2DBody"), joint.pleft);
}

void Box2DRopeJointDefinition_setLeft(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   VirtualObject& left = accessor.getObject(1);

   joint.pleft = &left.getNativeObject<Box2DBody>();
}

void Box2DRopeJointDefinition_getRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   RETURN_CLASS(UTEXT("box2d.Box2DBody"), joint.pright);
}

void Box2DRopeJointDefinition_setRight(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(Box2DRopeJointDefinition, joint);

   VirtualObject& right = accessor.getObject(1);

   joint.pright = &right.getNativeObject<Box2DBody>();
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
   
   RETURN_CLASS(UTEXT("engine.Process"), &map.getProcess());
}

void ActionMap_setProcess(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(ActionMap, map);

   Process& process = accessor.getObject(1).getNativeObject<Process>();

   map.setProcess(process);
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
   RETURN_CLASS(UTEXT("engine.io.FileSystem"), &FileSystem::getInstance());
}

void FileSystem_open(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   GET_THIS(FileSystem, fs);

   const String& name = accessor.getString(1);
   int modus = accessor.getInt(2);

   File* pfile = fs.open(name, modus);
   if ( pfile != NULL )
   {
      RETURN_CLASS_OWNED(UTEXT("engine.io.File"), pfile);
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

   registrator.addClass(UTEXT("engine.game.Process"));
   //registrator.addFunction(UTEXT("getFont"), Process_getFont);
   registrator.addFunction(UTEXT("getContentManager()"), Process_getContentManager);
   registrator.addFunction(UTEXT("setActionMap(engine.game.ActionMap)"), Process_setActionMap);
   registrator.addFunction(UTEXT("loadWorld(string)"), Process_loadWorld);
   registrator.addFunction(UTEXT("swapLeakDetection()"), Process_swapLeakDetection);

   registrator.addClass(UTEXT("engine.game.Server"));
   registrator.addFunction(UTEXT("listen(int)"), Server_listen);
   registrator.addFunction(UTEXT("sendScriptEvent(int, engine.net.NetStream)"), Server_sendScriptEvent);

   registrator.addClass(UTEXT("engine.game.Client"));
   registrator.addFunction(UTEXT("connect(string, int)"), Client_connect);
   registrator.addFunction(UTEXT("isActive()"), Client_isActive);
   registrator.addFunction(UTEXT("getWindowFactory()"), Client_getWindowFactory);
   registrator.addFunction(UTEXT("setWindow(system.GameWindow)"), Client_setWindow);
   registrator.addFunction(UTEXT("setKeyMap(engine.game.KeyMap)"), Client_setKeyMap);
   registrator.addFunction(UTEXT("getPlayer()"), Client_getPlayer);
   registrator.addFunction(UTEXT("getTexture(string)"), Client_getTexture);

   registrator.addClass(UTEXT("engine.game.ContentManager"));
   registrator.addFunction(UTEXT("loadEntity(string)"), ContentManager_loadEntity);

   registrator.addClass(UTEXT("system.GameWindowFactory"));
   registrator.addFunction(UTEXT("createWindow()"), GameWindowFactory_createWindow);

   registrator.addClass(UTEXT("system.GameWindow"));
   registrator.addFunction(UTEXT("create(string, int, int, int, boolean)"), GameWindow_create);

   registrator.addClass(UTEXT("engine.streams.BufferedStream"));
   registrator.addFunction(UTEXT("BufferedStream()"), BufferedStream_init);
   registrator.addFunction(UTEXT("finalize()"), BufferedStream_destruct);

   registrator.addClass(UTEXT("engine.net.NetStream"));
   registrator.addFunction(UTEXT("NetStream(engine.streams.BufferedStream)"), NetStream_init);
   registrator.addFunction(UTEXT("finalize()"), NetStream_destruct);
   registrator.addFunction(UTEXT("writeInt(int)"), NetStream_writeInt);
   registrator.addFunction(UTEXT("readInt()"), NetStream_readInt);
   registrator.addFunction(UTEXT("clear()"), NetStream_clear);

   registrator.addClass(UTEXT("engine.game.Entity"));
   registrator.addFunction(UTEXT("getId()"), Entity_getId);
   registrator.addFunction(UTEXT("sendComponentMessage(engine.game.ComponentMessage)"), Entity_sendComponentMessage);

   registrator.addClass(UTEXT("engine.game.QueryBodyComponentMessage"));
   registrator.addFunction(UTEXT("QueryBodyComponentMessage()"), QueryBodyComponentMessage_init);
   registrator.addFunction(UTEXT("hasBody()"), QueryBodyComponentMessage_hasBody);
   registrator.addFunction(UTEXT("getBody()"), QueryBodyComponentMessage_getBody);

   registrator.addClass(UTEXT("engine.game.AnimationComponentMessage"));
   registrator.addFunction(UTEXT("AnimationComponentMessage()"), AnimationComponentMessage_init);
   registrator.addFunction(UTEXT("setAnimation(int)"), AnimationComponentMessage_setAnimation);

   registrator.addClass(UTEXT("engine.game.Actor"));
   registrator.addFunction(UTEXT("Actor()"), Actor_init);
   registrator.addFunction(UTEXT("finalize()"), Actor_destruct);
   registrator.addFunction(UTEXT("getPositionX()"), Actor_getPositionX);
   registrator.addFunction(UTEXT("getPositionY()"), Actor_getPositionY);
   registrator.addFunction(UTEXT("setPosition(real, real)"), Actor_setPosition);
   registrator.addFunction(UTEXT("setName(string)"), Actor_setName);
   registrator.addFunction(UTEXT("direction()"), Actor_direction);
   registrator.addFunction(UTEXT("flip()"), Actor_flip);
   registrator.addFunction(UTEXT("setController(engine.game.Controller)"), Actor_setController);
   registrator.addFunction(UTEXT("add(engine.game.Actor)"), Actor_add);
   registrator.addFunction(UTEXT("hasLineOfSight(engine.game.Actor)"), Actor_hasLineOfSight);

   registrator.addClass(UTEXT("engine.game.Player"));
   registrator.addFunction(UTEXT("getClientId()"), Player_getClientId);
   registrator.addFunction(UTEXT("getController()"), Player_getController);
   registrator.addFunction(UTEXT("setController(engine.game.Actor)"), Player_setController);

   registrator.addClass(UTEXT("engine.game.World"));
   registrator.addFunction(UTEXT("World()"), World_init);
   registrator.addFunction(UTEXT("finalize()"), World_destruct);
   registrator.addFunction(UTEXT("getName()"), World_getName);
   registrator.addFunction(UTEXT("add(engine.game.Actor)"), World_add);
   registrator.addFunction(UTEXT("setObjectLayer(int)"), World_setObjectLayer);
   registrator.addFunction(UTEXT("setFollowMode(int)"), World_setFollowMode);
   registrator.addFunction(UTEXT("getFollowActor()"), World_getFollowActor);
   registrator.addFunction(UTEXT("setFollowActor(engine.game.Actor)"), World_setFollowActor);
   registrator.addFunction(UTEXT("setFollowBorders(int, int, int, int)"), World_setFollowBorders);
   registrator.addFunction(UTEXT("setFollowBorderWidth(int)"), World_setFollowBorderWidth);
   registrator.addFunction(UTEXT("getSimulator()"), World_getSimulator);
   registrator.addFunction(UTEXT("findEntity(int)"), World_findEntity);
   registrator.addFunction(UTEXT("getLayerCount()"), World_getLayerCount);
   registrator.addFunction(UTEXT("getLayer(int)"), World_getLayer);

   registrator.addClass(UTEXT("engine.game.Layer"));
   registrator.addFunction(UTEXT("getEffect()"), Layer_getEffect);
   registrator.addFunction(UTEXT("getWidth()"), Layer_getWidth);
   registrator.addFunction(UTEXT("getHeight()"), Layer_getHeight);
   registrator.addFunction(UTEXT("getTile(int, int)"), Layer_getTile);
   registrator.addFunction(UTEXT("setTile(int, int, int)"), Layer_setTile);

   registrator.addClass(UTEXT("engine.game.Effect"));

   registrator.addClass(UTEXT("engine.game.InputForceGenerator"));
   registrator.addFunction(UTEXT("InputForceGenerator()"), InputForceGenerator_init);
   registrator.addFunction(UTEXT("finalize()"), InputForceGenerator_destruct);
   registrator.addFunction(UTEXT("getVelocity()"), InputForceGenerator_getVelocity);
   registrator.addFunction(UTEXT("setVelocity(real, real)"), InputForceGenerator_setVelocity);
   registrator.addFunction(UTEXT("setImpulse(real, real)"), InputForceGenerator_setImpulse);

   registrator.addClass(UTEXT("engine.game.InputController"));
   registrator.addFunction(UTEXT("InputController()"), InputController_init);
   registrator.addFunction(UTEXT("finalize()"), InputController_destruct);
   registrator.addFunction(UTEXT("setActionMap(engine.game.ActionMap)"), InputController_setActionMap);

   registrator.addClass(UTEXT("engine.game.AIController"));
   registrator.addFunction(UTEXT("AIController()"), AIController_init);
   registrator.addFunction(UTEXT("finalize()"), AIController_destruct);

   registrator.addClass(UTEXT("box2d.Box2DSimulator"));
   registrator.addFunction(UTEXT("lineOfSight(Actor, Actor)"), Box2DSimulator_lineOfSight);
   registrator.addFunction(UTEXT("createRevoluteJoint(box2d.Box2DBody, box2d.Box2DBody, real, real)"), Box2DSimulator_createRevoluteJoint);
   registrator.addFunction(UTEXT("createRopeJoint(box2d.Box2DRopeJointDefinition)"), Box2DSimulator_createRopeJoint);

   registrator.addClass(UTEXT("box2d.Box2DBody"));
   registrator.addFunction(UTEXT("addForceGenerator(engine.game.ForceGenerator)"), Box2DBody_addForceGenerator);

   registrator.addClass(UTEXT("box2d.Box2DRopeJointDefinition"));
   registrator.addFunction(UTEXT("Box2DRopeJointDefinition"), Box2DRopeJointDefinition_init);
   registrator.addFunction(UTEXT("finalize()"), Box2DRopeJointDefinition_destruct);
   registrator.addFunction(UTEXT("getLeft()"), Box2DRopeJointDefinition_getLeft);
   registrator.addFunction(UTEXT("setLeft(box2d.Box2DBody)"), Box2DRopeJointDefinition_setLeft);
   registrator.addFunction(UTEXT("getRight()"), Box2DRopeJointDefinition_getRight);
   registrator.addFunction(UTEXT("setRight(box2d.Box2DBody)"), Box2DRopeJointDefinition_setRight);
   registrator.addFunction(UTEXT("getLocalAnchorLeftX()"), Box2DRopeJointDefinition_getLocalAnchorLeftX);
   registrator.addFunction(UTEXT("getLocalAnchorLeftY()"), Box2DRopeJointDefinition_getLocalAnchorLeftY);
   registrator.addFunction(UTEXT("setLocalAnchorLeft(real, real)"), Box2DRopeJointDefinition_setLocalAnchorLeft);
   registrator.addFunction(UTEXT("getLocalAnchorRightX()"), Box2DRopeJointDefinition_getLocalAnchorRightX);
   registrator.addFunction(UTEXT("getLocalAnchorRightY()"), Box2DRopeJointDefinition_getLocalAnchorRightY);
   registrator.addFunction(UTEXT("setLocalAnchorRight(real, real)"), Box2DRopeJointDefinition_setLocalAnchorRight);

   registrator.addClass(UTEXT("engine.game.ActionMap"));
   registrator.addFunction(UTEXT("ActionMap()"), ActionMap_init);
   registrator.addFunction(UTEXT("finalize()"), ActionMap_destruct);
   registrator.addFunction(UTEXT("getProcess()"), ActionMap_getProcess);
   registrator.addFunction(UTEXT("setProcess(engine.game.Process)"), ActionMap_setProcess);
   registrator.addFunction(UTEXT("bind(int, string)"), ActionMap_bind);

   registrator.addClass(UTEXT("engine.game.KeyMap"));
   registrator.addFunction(UTEXT("KeyMap()"), KeyMap_init);
   registrator.addFunction(UTEXT("finalize()"), KeyMap_destruct);
   registrator.addFunction(UTEXT("bind(int, int)"), KeyMap_bind);

   /*
   registrator.addFunction(UTEXT("EngineGraphics_init"), EngineGraphics_init);
   registrator.addFunction(UTEXT("EngineGraphics_destruct"), EngineGraphics_destruct);
   registrator.addFunction(UTEXT("EngineGraphics_doSetColor"), EngineGraphics_doSetColor);
   registrator.addFunction(UTEXT("EngineGraphics_nativeSetFont"), EngineGraphics_nativeSetFont);
   registrator.addFunction(UTEXT("EngineGraphics_translate"), EngineGraphics_translate);
   registrator.addFunction(UTEXT("EngineGraphics_drawText"), EngineGraphics_drawText);
   registrator.addFunction(UTEXT("EngineGraphics_native_fillRect"), EngineGraphics_native_fillRect);
   registrator.addFunction(UTEXT("EngineGraphics_native_drawRect"), EngineGraphics_native_drawRect);
   registrator.addFunction(UTEXT("EngineGraphics_native_drawRoundedRect"), EngineGraphics_native_drawRoundedRect);
   registrator.addFunction(UTEXT("EngineGraphics_native_drawTexture"), EngineGraphics_native_drawTexture);
   */

   registrator.addClass(UTEXT("engine.ui.Font"));
   registrator.addFunction(UTEXT("finalize()"), Font_destruct);
   registrator.addFunction(UTEXT("render(string)"), Font_render);
   registrator.addFunction(UTEXT("getBaseLine()"), Font_getBaseLine);
   registrator.addFunction(UTEXT("getTextWidth(string)"), Font_getTextWidth);
   registrator.addFunction(UTEXT("getTextHeight(string)"), Font_getTextHeight);

   registrator.addClass(UTEXT("engine.core.Texture"));
   registrator.addFunction(UTEXT("finalize()"), Texture_destruct);
   registrator.addFunction(UTEXT("getName()"), Texture_getName);
   registrator.addFunction(UTEXT("getWidth()"), Texture_getWidth);
   registrator.addFunction(UTEXT("getHeight()"), Texture_getHeight);
   registrator.addFunction(UTEXT("getSourceWidth()"), Texture_getSourceWidth);
   registrator.addFunction(UTEXT("getSourceHeight()"), Texture_getSourceHeight);

   registrator.addClass(UTEXT("engine.io.FileSystem"));
   registrator.addFunction(UTEXT("getInstance()"), FileSystem_getInstance);
   registrator.addFunction(UTEXT("open(string, int)"), FileSystem_open);

   registrator.addClass(UTEXT("engine.io.File"));
   registrator.addFunction(UTEXT("finalize()"), File_destruct);
   registrator.addFunction(UTEXT("length()"), File_length);
   registrator.addFunction(UTEXT("readText()"), File_readText);

   registrator.registerCallbacks(manager);
}
