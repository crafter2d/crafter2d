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
#include "core/content/contentmanager.h"
#include "core/entity/entity.h"
#include "core/entity/componentmessages/querybodycomponentmessage.h"
#include "core/entity/componentmessages/animationcomponentmessage.h"
#include "core/graphics/effect.h"
#include "core/graphics/texture.h"
#include "core/graphics/viewport.h"
#include "core/physics/inputforcegenerator.h"
#include "core/physics/simulator.h"
#include "core/resource/resourcemanager.h"
#include "core/smartptr/autoptr.h"
#include "core/streams/datastream.h"
#include "core/streams/bufferedstream.h"
#include "core/script/scriptcall.h"
#include "core/script/scriptmanager.h"
#include "core/script/scriptregistrator.h"
#include "core/script/scriptobject.h"
#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/world/layer.h"
#include "core/world/world.h"
#include "core/window/gamewindowfactory.h"
#include "core/window/gamewindow.h"
#include "core/defines.h"

#include "engine/net/netstream.h"

#include "physics/box2d/box2dbody.h"
#include "physics/box2d/box2dsimulator.h"
#include "physics/box2d/box2drevolutejoint.h"
#include "physics/box2d/box2dropejoint.h"

#include "actionmap.h"
#include "keymap.h"
#include "client.h"
#include "player.h"
#include "server.h"
#include "inputcontroller.h"
#include "aicontroller.h"

using namespace c2d;

#define GET_THIS(type, variable)                   type& variable = accessor.getObject(0)->get<type>()
#define DESTRUCT_THIS(type)                        delete accessor.getObject(0)->use<type>();

#define RETURN_CLASS(class, pointer)               accessor.setResult(class, pointer, false)
#define RETURN_CLASS_OWNED(class, pointer)         accessor.setResult(class, pointer, true)

#define RETURN_CLASS_I(scriptable)                 accessor.setResult(scriptable, false);

void Process_getContentManager(ScriptCall& accessor)
{
   GET_THIS(Process, process);

   RETURN_CLASS(UTEXT("engine.game.ContentManager"), &process.getContentManager());
}

void Process_setActionMap(ScriptCall& accessor)
{
   GET_THIS(Server, server);

   ActionMap* pmap = accessor.getObject(1)->use<ActionMap>();

   server.setActionMap(pmap);
}

void Process_loadWorld(ScriptCall& accessor)
{
   GET_THIS(Process, process);

   const String& filename = accessor.getString(1);
   World* pworld = process.loadWorld(filename);

   RETURN_CLASS(UTEXT("engine.game.World"), pworld);
}

void Process_swapLeakDetection(ScriptCall& accessor)
{
   GET_THIS(Process, process);

   process.swapLeakDetection();
}

void Server_listen(ScriptCall& accessor)
{
   GET_THIS(Server, server);

   int port = accessor.getInt(1);

   accessor.setResult(server.listen(port));
}

void Server_sendScriptEvent(ScriptCall& accessor)
{
   GET_THIS(Server, server);

   int client = accessor.getInt(1);
   NetStream& stream = accessor.getObject(2)->get<NetStream>();

   server.sendScriptEvent(client, stream);
}

void Client_connect(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   String ip = accessor.getString(1);
   int port = accessor.getInt(2);

   accessor.setResult(client.connect(ip, port));
}

void Client_isActive(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   accessor.setResult(client.isActive());
}

void Client_getWindowFactory(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   RETURN_CLASS(UTEXT("system.GameWindowFactory"), &client.getWindowFactory());
}

void Client_setWindow(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   GameWindow* pwindow = accessor.getObject(1)->use<GameWindow>();

   client.setWindow(pwindow);
}

void Client_getPlayer(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   Player& player = client.getPlayer();
   RETURN_CLASS(UTEXT("engine.game.Player"), &player);
}

void Client_setPlayer(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   Player* pplayer = accessor.getObject(1)->use<Player>();
   client.setPlayer(pplayer);
}

void Client_setActionMap(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   ActionMap* pmap = accessor.getObject(1)->use<ActionMap>();

   client.setActionMap(pmap);
}

void Client_setKeyMap(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   KeyMap* pmap = accessor.getObject(1)->use<KeyMap>();

   client.setKeyMap(pmap);
}

void Client_getTexture(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   const String& name = accessor.getString(1);

   TexturePtr* ptexture = new TexturePtr(ResourceManager::getInstance().getTexture(name));
   RETURN_CLASS_OWNED(UTEXT("engine.core.Texture"), ptexture);
}

void Client_getViewport(ScriptCall& accessor)
{
   GET_THIS(Client, client);

   RETURN_CLASS(UTEXT("engine.game.Viewport"), &client.getViewport());
}

void ContentManager_loadEntity(ScriptCall& accessor)
{
   GET_THIS(ContentManager, contentmanager);

   const String& filename = accessor.getString(1);

   Entity* presult = contentmanager.loadContent<Entity>(filename);
   RETURN_CLASS_OWNED(presult->getClassName(), presult);
}

void GameWindowFactory_createWindow(ScriptCall& accessor)
{
   GET_THIS(GameWindowFactory, factory);

   RETURN_CLASS_OWNED(UTEXT("system.GameWindow"), factory.createWindow());
}

void GameWindow_create(ScriptCall& accessor)
{
   GET_THIS(GameWindow, window);

   const String& title = accessor.getString(1);
   int width = accessor.getInt(2);
   int height = accessor.getInt(3);
   int bitdepth = accessor.getInt(4);
   bool fullscreen = accessor.getBoolean(5);

   accessor.setResult(window.create(title, width, height, bitdepth, fullscreen));
}

void BufferedStream_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new BufferedStream());
}

void BufferedStream_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(BufferedStream);
}

void NetStream_init(ScriptCall& accessor)
{
   BufferedStream& bufferedstream = accessor.getObject(1)->get<BufferedStream>();
   NetStream* pstream = new NetStream(bufferedstream);

   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(pstream);
}

void NetStream_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(NetStream);
}

void NetStream_writeInt(ScriptCall& accessor)
{
   GET_THIS(NetStream, stream);

   int value = accessor.getInt(1);

   stream.writeInt(value);
}

void NetStream_readInt(ScriptCall& accessor)
{
   GET_THIS(NetStream, stream);

   int value;
   stream.readInt(value);

   accessor.setResult(value);
}

void NetStream_clear(ScriptCall& accessor)
{
   GET_THIS(NetStream, stream);

   stream.clear();
}

void QueryBodyComponentMessage_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new QueryBodyComponentMessage());
}

void QueryBodyComponentMessage_hasBody(ScriptCall& accessor)
{
   GET_THIS(QueryBodyComponentMessage, msg);

   accessor.setResult(msg.hasBody());
}

void QueryBodyComponentMessage_getBody(ScriptCall& accessor)
{
   GET_THIS(QueryBodyComponentMessage, msg);

   RETURN_CLASS_I(msg.getBody());
}

void AnimationComponentMessage_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new AnimationComponentMessage());
}

void AnimationComponentMessage_setAnimation(ScriptCall& accessor)
{
   GET_THIS(AnimationComponentMessage, msg);

   msg.setAnimation(accessor.getInt(1));
}

void Entity_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new Entity());
}

void Entity_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(Entity);
}

void Entity_getId(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   accessor.setResult((int)entity.getId());
}

void Entity_sendComponentMessage(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   ComponentMessage& msg = accessor.getObject(1)->get<ComponentMessage>();
   entity.sendComponentMessage(msg);
}

void Entity_hasLineOfSight(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   Entity& other = accessor.getObject(1)->get<Entity>();
   accessor.setResult(entity.hasLineOfSight(other));
}

void Entity_getPositionX(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   accessor.setResult(entity.getPosition().x);
}

void Entity_getPositionY(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   accessor.setResult(entity.getPosition().y);
}

void Entity_setPosition(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   float xpos = accessor.getReal(1);
   float ypos = accessor.getReal(2);

   entity.setPosition(Vector(xpos, ypos));
}

void Entity_setName(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   const String& name = accessor.getString(1);

   entity.setName(name);
}

void Entity_flip(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   entity.flip();
}

void Entity_setController(ScriptCall& accessor)
{
   GET_THIS(Entity, entity);

   Controller* pcontroller = accessor.getObject(1)->use<Controller>();

   entity.setController(pcontroller);
}

void Player_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new Player());
}

void Player_getClientId(ScriptCall& accessor)
{
   GET_THIS(Player, player);

   accessor.setResult(player.getClientId());
}

void Player_getController(ScriptCall& accessor)
{
   GET_THIS(Player, player);

   RETURN_CLASS(UTEXT("engine.game.Entity"), &player.getController());
}

void Player_setController(ScriptCall& accessor)
{
   GET_THIS(Player, player);

   Entity& entity = accessor.getObject(1)->get<Entity>();

   player.setController(entity);
}

void World_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new World());
}

void World_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(World);
}

void World_getName(ScriptCall& accessor)
{
   GET_THIS(World, world);

   accessor.setResult(world.getName());
}

void World_add(ScriptCall& accessor)
{
   GET_THIS(World, world);

   Entity* pentity = accessor.getObject(1)->use<Entity>();

   world.addEntity(pentity);
}

void World_setObjectLayer(ScriptCall& accessor)
{
   GET_THIS(World, world);

   int layer = accessor.getInt(1);

   world.setObjectLayer(layer);
}

void World_setFollowMode(ScriptCall& accessor)
{
   GET_THIS(World, world);

   int mode = accessor.getInt(1);

   world.setScrollMode(mode);
}

void World_getFollowActor(ScriptCall& accessor)
{
   GET_THIS(World, world);
      
   RETURN_CLASS(UTEXT("engine.game.Entity"), world.getFollowObject());
}

void World_setFollowActor(ScriptCall& accessor)
{
   GET_THIS(World, world);

   Entity& entity = accessor.getObject(1)->get<Entity>();

   world.setFollowObject(entity);
}

void World_getSimulator(ScriptCall& accessor)
{
   GET_THIS(World, world);

   RETURN_CLASS_I(world.getSimulator());
}

void World_setFollowBorders(ScriptCall& accessor)
{
   GET_THIS(World, world);

   int left = accessor.getInt(1);
   int right = accessor.getInt(2);
   int top = accessor.getInt(3);
   int bottom = accessor.getInt(4);

   world.setFollowBorders(left, right, top, bottom);
}

void World_setFollowBorderWidth(ScriptCall& accessor)
{
   GET_THIS(World, world);

   int width = accessor.getInt(1);
   
   world.setFollowBorderWidth(width);
}

void World_findEntity(ScriptCall& accessor)
{
   GET_THIS(World, world);

   int controllerid = accessor.getInt(1);
   Entity* pentity = world.findEntity(Id(controllerid));

   RETURN_CLASS(UTEXT("engine.game.Actor"), pentity);
}

void World_getLayerCount(ScriptCall& accessor)
{
   GET_THIS(World, world);

   accessor.setResult(world.getLayerCount());
}
 
void World_getLayer(ScriptCall& accessor)
{
   GET_THIS(World, world);

   int index = accessor.getInt(1);

   RETURN_CLASS(UTEXT("engine.game.Layer"), world.getLayer(index));
}

void Layer_getEffect(ScriptCall& accessor)
{
   GET_THIS(Layer, layer);

   RETURN_CLASS(UTEXT("engine.game.Effect"), const_cast<Graphics::Effect*>(&layer.getEffect()));
}

void Layer_getWidth(ScriptCall& accessor)
{
   GET_THIS(Layer, layer);

   accessor.setResult(layer.getWidth());
}

void Layer_getHeight(ScriptCall& accessor)
{
   GET_THIS(Layer, layer);

   accessor.setResult(layer.getHeight());
}

void Layer_getTile(ScriptCall& accessor)
{
   GET_THIS(Layer, layer);

   int level = accessor.getInt(1);
   int x = accessor.getInt(2);
   int y = accessor.getInt(3);

   accessor.setResult(layer.getTile((LayerLevel)level, x, y));
}

void Layer_setTile(ScriptCall& accessor)
{
   GET_THIS(Layer, layer);

   int level = accessor.getInt(1);
   int x = accessor.getInt(2);
   int y = accessor.getInt(3);
   int t = accessor.getInt(4);

   layer.setTile((LayerLevel)level, x, y, t);
}

void Viewport_getWidth(ScriptCall& accessor)
{
   GET_THIS(Graphics::Viewport, viewport);

   accessor.setResult(viewport.getWidth());
}

void Viewport_getHeight(ScriptCall& accessor)
{
   GET_THIS(Graphics::Viewport, viewport);

   accessor.setResult(viewport.getHeight());
}

void InputForceGenerator_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new InputForceGenerator());
}

void InputForceGenerator_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(InputForceGenerator);
}

void InputForceGenerator_getVelocity(ScriptCall& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   ScriptObjectHandle object = accessor.newObject(UTEXT("engine.core.Vector2D"));
   object->setMember(0, Variant(generator.getVelocity().x));
   object->setMember(1, Variant(generator.getVelocity().y));

   accessor.setResult(object);
}

void InputForceGenerator_setVelocity(ScriptCall& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   float xvel = accessor.getReal(1);
   float yvel = accessor.getReal(2);

   generator.setVelocity(Vector(xvel, yvel));
}

void InputForceGenerator_setImpulse(ScriptCall& accessor)
{
   GET_THIS(InputForceGenerator, generator);

   float ximp = accessor.getReal(1);
   float yimp = accessor.getReal(2);

   generator.setImpulse(Vector(ximp, yimp));
}

void InputController_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new InputController());
}

void InputController_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(InputController);
}

void InputController_setActionMap(ScriptCall& accessor)
{
   GET_THIS(InputController, controller);

   ActionMap& actionmap = accessor.getObject(1)->get<ActionMap>();

   controller.setActionMap(actionmap);
}

void AIController_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);

   AIController* pcontroller = new AIController();
   pcontroller->setThis(thisobject.use());
   
   thisobject->setInstance(pcontroller);
}

void AIController_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(AIController);
}

void Box2DSimulator_lineOfSight(ScriptCall& accessor)
{
   //GET_THIS(Box2DSimulator, simulator);

   //const Actor& from = accessor.getObject(1).getNativeObject<Actor>();
   //const Actor& to = accessor.getObject(2).getNativeObject<Actor>();

   accessor.setResult(false); //simulator.lineOfSight(from.getBody(), to.getBody()));
}

void Box2DSimulator_createRevoluteJoint(ScriptCall& accessor)
{
   C2D_UNUSED(accessor);
   /*
   GET_THIS(Box2DSimulator, simulator);

   Box2DBody& left = accessor.getObject(1).getNativeObject<Box2DBody>();
   Box2DBody& right = accessor.getObject(2).getNativeObject<Box2DBody>();
   float anchorx = accessor.getReal(3);
   float anchory = accessor.getReal(4);

   simulator.createRevoluteJoint(left, right, Vector(anchorx, anchory));
   */
}

void Box2DSimulator_createRopeJoint(ScriptCall& accessor)
{
   C2D_UNUSED(accessor);
   /*
   GET_THIS(Box2DSimulator, simulator);

   Box2DRopeJointDefinition& jointdef = accessor.getObject(1).getNativeObject<Box2DRopeJointDefinition>();

   simulator.createRopeJoint(jointdef);
   */
}

void Box2DBody_addForceGenerator(ScriptCall& accessor)
{
   GET_THIS(Box2DBody, body);

   ForceGenerator* pgenerator = accessor.getObject(1)->use<ForceGenerator>();

   body.addForceGenerator(pgenerator);
}

void Box2DBody_getMass(ScriptCall& accessor)
{
   GET_THIS(Box2DBody, body);

   accessor.setResult(body.getMass());
}

void Box2DBody_getLinearVelocity(ScriptCall& accessor)
{
   GET_THIS(Box2DBody, body);

   Vector vel = body.getLinearVelocity();

   ScriptObjectHandle object = accessor.newObject(UTEXT("engine.core.Vector2D"));
   object->setMember(0, Variant(vel.x));
   object->setMember(1, Variant(vel.y));

   accessor.setResult(object);
}

void ActionMap_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new ActionMap());
}

void ActionMap_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(ActionMap);
}

void ActionMap_getProcess(ScriptCall& accessor)
{
   GET_THIS(ActionMap, map);
   
   RETURN_CLASS(UTEXT("engine.Process"), &map.getProcess());
}

void ActionMap_setProcess(ScriptCall& accessor)
{
   GET_THIS(ActionMap, map);

   Process& process = accessor.getObject(1)->get<Process>();

   map.setProcess(process);
}

void ActionMap_bind(ScriptCall& accessor)
{
   GET_THIS(ActionMap, map);

   int action = accessor.getInt(1);
   const String& fnc = accessor.getString(2);

   map.bind(action, fnc);
}

void KeyMap_init(ScriptCall& accessor)
{
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(new KeyMap);
}

void KeyMap_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(KeyMap);
}

void KeyMap_bind(ScriptCall& accessor)
{
   GET_THIS(KeyMap, map);

   int key = accessor.getInt(1);
   int action = accessor.getInt(2);

   map.bind(key, action);
}

void Texture_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(TexturePtr);
}

void Texture_getName(ScriptCall& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getName());
}

void Texture_getWidth(ScriptCall& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getWidth());
}

void Texture_getHeight(ScriptCall& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getHeight());
}

void Texture_getSourceWidth(ScriptCall& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getSourceWidth());
}

void Texture_getSourceHeight(ScriptCall& accessor)
{
   GET_THIS(TexturePtr, texture);

   accessor.setResult(texture->getSourceHeight());
}

void FileSystem_getInstance(ScriptCall& accessor)
{
   RETURN_CLASS(UTEXT("engine.io.FileSystem"), &FileSystem::getInstance());
}

void FileSystem_open(ScriptCall& accessor)
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

void File_destruct(ScriptCall& accessor)
{
   DESTRUCT_THIS(File);
}

void File_length(ScriptCall& accessor)
{
   GET_THIS(File, file);

   accessor.setResult(file.size());
}

void File_readText(ScriptCall& accessor)
{
   GET_THIS(File, file);

   int length = file.size();
   char* pbuffer = new char[length];
   length = file.read(pbuffer, length);

   /*
   VirtualArray* parray = machine.instantiateArray();
   parray->addLevel(length);
   
   for ( int index = 0; index < length; index++ )
   {
      (*parray)[index].setChar(pbuffer[index]);
   }
   */

   delete[] pbuffer;

   //accessor.setResult(*parray);
}

// - Registration

void script_engine_register(c2d::ScriptManager& manager)
{
   std::unique_ptr<ScriptRegistrator> pregistrator(manager.getRegistrator());

   pregistrator->addClass(UTEXT("engine.game.Process"));
   pregistrator->addFunction(UTEXT("getContentManager()"), Process_getContentManager);
   pregistrator->addFunction(UTEXT("setActionMap(engine.game.ActionMap)"), Process_setActionMap);
   pregistrator->addFunction(UTEXT("loadWorld(string)"), Process_loadWorld);
   pregistrator->addFunction(UTEXT("swapLeakDetection()"), Process_swapLeakDetection);

   pregistrator->addClass(UTEXT("engine.game.Server"));
   pregistrator->addFunction(UTEXT("listen(int)"), Server_listen);
   pregistrator->addFunction(UTEXT("sendScriptEvent(int, engine.net.NetStream)"), Server_sendScriptEvent);

   pregistrator->addClass(UTEXT("engine.game.Client"));
   pregistrator->addFunction(UTEXT("connect(string, int)"), Client_connect);
   pregistrator->addFunction(UTEXT("isActive()"), Client_isActive);
   pregistrator->addFunction(UTEXT("getWindowFactory()"), Client_getWindowFactory);
   pregistrator->addFunction(UTEXT("setWindow(system.GameWindow)"), Client_setWindow);
   pregistrator->addFunction(UTEXT("setKeyMap(engine.game.KeyMap)"), Client_setKeyMap);
   pregistrator->addFunction(UTEXT("getPlayer()"), Client_getPlayer);
   pregistrator->addFunction(UTEXT("setPlayer(engine.game.Player)"), Client_setPlayer);
   pregistrator->addFunction(UTEXT("getTexture(string)"), Client_getTexture);
   pregistrator->addFunction(UTEXT("getViewport()"), Client_getViewport);

   pregistrator->addClass(UTEXT("engine.game.ContentManager"));
   pregistrator->addFunction(UTEXT("loadEntity(string)"), ContentManager_loadEntity);

   pregistrator->addClass(UTEXT("system.GameWindowFactory"));
   pregistrator->addFunction(UTEXT("createWindow()"), GameWindowFactory_createWindow);

   pregistrator->addClass(UTEXT("system.GameWindow"));
   pregistrator->addFunction(UTEXT("create(string, int, int, int, boolean)"), GameWindow_create);

   pregistrator->addClass(UTEXT("engine.streams.BufferedStream"));
   pregistrator->addFunction(UTEXT("BufferedStream()"), BufferedStream_init);
   pregistrator->addFunction(UTEXT("finalize()"), BufferedStream_destruct);

   pregistrator->addClass(UTEXT("engine.net.NetStream"));
   pregistrator->addFunction(UTEXT("NetStream(engine.streams.BufferedStream)"), NetStream_init);
   pregistrator->addFunction(UTEXT("finalize()"), NetStream_destruct);
   pregistrator->addFunction(UTEXT("writeInt(int)"), NetStream_writeInt);
   pregistrator->addFunction(UTEXT("readInt()"), NetStream_readInt);
   pregistrator->addFunction(UTEXT("clear()"), NetStream_clear);

   pregistrator->addClass(UTEXT("engine.game.Entity"));
   pregistrator->addFunction(UTEXT("Entity()"), Entity_init);
   pregistrator->addFunction(UTEXT("finalize()"), Entity_destruct);
   pregistrator->addFunction(UTEXT("getId()"), Entity_getId);
   pregistrator->addFunction(UTEXT("getPositionX()"), Entity_getPositionX);
   pregistrator->addFunction(UTEXT("getPositionY()"), Entity_getPositionY);
   pregistrator->addFunction(UTEXT("setPosition(real, real)"), Entity_setPosition);
   pregistrator->addFunction(UTEXT("setName(string)"), Entity_setName);
   pregistrator->addFunction(UTEXT("flip()"), Entity_flip);
   pregistrator->addFunction(UTEXT("setController(engine.game.Controller)"), Entity_setController);
   pregistrator->addFunction(UTEXT("sendComponentMessage(engine.game.ComponentMessage)"), Entity_sendComponentMessage);
   pregistrator->addFunction(UTEXT("hasLineOfSight(engine.game.Entity)"), Entity_hasLineOfSight);

   pregistrator->addClass(UTEXT("engine.game.QueryBodyComponentMessage"));
   pregistrator->addFunction(UTEXT("QueryBodyComponentMessage()"), QueryBodyComponentMessage_init);
   pregistrator->addFunction(UTEXT("hasBody()"), QueryBodyComponentMessage_hasBody);
   pregistrator->addFunction(UTEXT("getBody()"), QueryBodyComponentMessage_getBody);

   pregistrator->addClass(UTEXT("engine.game.AnimationComponentMessage"));
   pregistrator->addFunction(UTEXT("AnimationComponentMessage()"), AnimationComponentMessage_init);
   pregistrator->addFunction(UTEXT("setAnimation(int)"), AnimationComponentMessage_setAnimation);

   pregistrator->addClass(UTEXT("engine.game.Player"));
   pregistrator->addFunction(UTEXT("Player()"), Player_init);
   pregistrator->addFunction(UTEXT("getClientId()"), Player_getClientId);
   pregistrator->addFunction(UTEXT("getController()"), Player_getController);
   pregistrator->addFunction(UTEXT("setController(engine.game.Entity)"), Player_setController);

   pregistrator->addClass(UTEXT("engine.game.World"));
   pregistrator->addFunction(UTEXT("World()"), World_init);
   pregistrator->addFunction(UTEXT("finalize()"), World_destruct);
   pregistrator->addFunction(UTEXT("getName()"), World_getName);
   pregistrator->addFunction(UTEXT("add(engine.game.Entity)"), World_add);
   pregistrator->addFunction(UTEXT("setObjectLayer(int)"), World_setObjectLayer);
   pregistrator->addFunction(UTEXT("setFollowMode(int)"), World_setFollowMode);
   pregistrator->addFunction(UTEXT("getFollowActor()"), World_getFollowActor);
   pregistrator->addFunction(UTEXT("setFollowActor(engine.game.Entity)"), World_setFollowActor);
   pregistrator->addFunction(UTEXT("setFollowBorders(int, int, int, int)"), World_setFollowBorders);
   pregistrator->addFunction(UTEXT("setFollowBorderWidth(int)"), World_setFollowBorderWidth);
   pregistrator->addFunction(UTEXT("getSimulator()"), World_getSimulator);
   pregistrator->addFunction(UTEXT("findEntity(int)"), World_findEntity);
   pregistrator->addFunction(UTEXT("getLayerCount()"), World_getLayerCount);
   pregistrator->addFunction(UTEXT("getLayer(int)"), World_getLayer);

   pregistrator->addClass(UTEXT("engine.game.Layer"));
   pregistrator->addFunction(UTEXT("getEffect()"), Layer_getEffect);
   pregistrator->addFunction(UTEXT("getWidth()"), Layer_getWidth);
   pregistrator->addFunction(UTEXT("getHeight()"), Layer_getHeight);
   pregistrator->addFunction(UTEXT("getTile(int, int)"), Layer_getTile);
   pregistrator->addFunction(UTEXT("setTile(int, int, int)"), Layer_setTile);

   pregistrator->addClass(UTEXT("engine.game.Effect"));

   pregistrator->addClass(UTEXT("engine.game.Viewport"));
   pregistrator->addFunction(UTEXT("getWidth()"), Viewport_getWidth);
   pregistrator->addFunction(UTEXT("getHeight()"), Viewport_getHeight);

   pregistrator->addClass(UTEXT("engine.game.InputForceGenerator"));
   pregistrator->addFunction(UTEXT("InputForceGenerator()"), InputForceGenerator_init);
   pregistrator->addFunction(UTEXT("finalize()"), InputForceGenerator_destruct);
   pregistrator->addFunction(UTEXT("getVelocity()"), InputForceGenerator_getVelocity);
   pregistrator->addFunction(UTEXT("setVelocity(real, real)"), InputForceGenerator_setVelocity);
   pregistrator->addFunction(UTEXT("setImpulse(real, real)"), InputForceGenerator_setImpulse);

   pregistrator->addClass(UTEXT("engine.game.InputController"));
   pregistrator->addFunction(UTEXT("InputController()"), InputController_init);
   pregistrator->addFunction(UTEXT("finalize()"), InputController_destruct);
   pregistrator->addFunction(UTEXT("setActionMap(engine.game.ActionMap)"), InputController_setActionMap);

   pregistrator->addClass(UTEXT("engine.game.AIController"));
   pregistrator->addFunction(UTEXT("AIController()"), AIController_init);
   pregistrator->addFunction(UTEXT("finalize()"), AIController_destruct);

   pregistrator->addClass(UTEXT("box2d.Box2DSimulator"));
   pregistrator->addFunction(UTEXT("lineOfSight(engine.game.Entity, engine.game.Entity)"), Box2DSimulator_lineOfSight);
   pregistrator->addFunction(UTEXT("createRevoluteJoint(box2d.Box2DBody, box2d.Box2DBody, real, real)"), Box2DSimulator_createRevoluteJoint);
   pregistrator->addFunction(UTEXT("createRopeJoint(box2d.Box2DRopeJointDefinition)"), Box2DSimulator_createRopeJoint);

   pregistrator->addClass(UTEXT("box2d.Box2DBody"));
   pregistrator->addFunction(UTEXT("addForceGenerator(engine.game.ForceGenerator)"), Box2DBody_addForceGenerator);
   pregistrator->addFunction(UTEXT("getMass()"), Box2DBody_getMass);
   pregistrator->addFunction(UTEXT("getLinearVelocity()"), Box2DBody_getLinearVelocity);
   
   pregistrator->addClass(UTEXT("engine.game.ActionMap"));
   pregistrator->addFunction(UTEXT("ActionMap()"), ActionMap_init);
   pregistrator->addFunction(UTEXT("finalize()"), ActionMap_destruct);
   pregistrator->addFunction(UTEXT("getProcess()"), ActionMap_getProcess);
   pregistrator->addFunction(UTEXT("setProcess(engine.game.Process)"), ActionMap_setProcess);
   pregistrator->addFunction(UTEXT("bind(int, string)"), ActionMap_bind);

   pregistrator->addClass(UTEXT("engine.game.KeyMap"));
   pregistrator->addFunction(UTEXT("KeyMap()"), KeyMap_init);
   pregistrator->addFunction(UTEXT("finalize()"), KeyMap_destruct);
   pregistrator->addFunction(UTEXT("bind(int, int)"), KeyMap_bind);

   pregistrator->addClass(UTEXT("engine.core.Texture"));
   pregistrator->addFunction(UTEXT("finalize()"), Texture_destruct);
   pregistrator->addFunction(UTEXT("getName()"), Texture_getName);
   pregistrator->addFunction(UTEXT("getWidth()"), Texture_getWidth);
   pregistrator->addFunction(UTEXT("getHeight()"), Texture_getHeight);
   pregistrator->addFunction(UTEXT("getSourceWidth()"), Texture_getSourceWidth);
   pregistrator->addFunction(UTEXT("getSourceHeight()"), Texture_getSourceHeight);

   pregistrator->addClass(UTEXT("engine.io.FileSystem"));
   pregistrator->addFunction(UTEXT("getInstance()"), FileSystem_getInstance);
   pregistrator->addFunction(UTEXT("open(string, int)"), FileSystem_open);

   pregistrator->addClass(UTEXT("engine.io.File"));
   pregistrator->addFunction(UTEXT("finalize()"), File_destruct);
   pregistrator->addFunction(UTEXT("length()"), File_length);
   pregistrator->addFunction(UTEXT("readText()"), File_readText);

   pregistrator->registerCallbacks();
}
