/*
** Lua binding: network
** Generated automatically by tolua++-1.0.92 on 12/29/08 11:10:27.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_network_open (lua_State* tolua_S);

#include "net/netconnection.h"
#include "server.h"
#include "client.h"
#include "actionmap.h"
#include "process.h"
#include "scenegraph.h"
#include "player.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_NetConnection (lua_State* tolua_S)
{
 NetConnection* self = (NetConnection*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_BitStream (lua_State* tolua_S)
{
 BitStream* self = (BitStream*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Player (lua_State* tolua_S)
{
 Player* self = (Player*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"NetConnection");
 tolua_usertype(tolua_S,"ActionMap");
 tolua_usertype(tolua_S,"SceneGraph");
 tolua_usertype(tolua_S,"Player");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"Process");
 tolua_usertype(tolua_S,"Client");
 tolua_usertype(tolua_S,"BitStream");
 tolua_usertype(tolua_S,"Server");
}

/* method: new of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_new00
static int tolua_network_BitStream_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BitStream* tolua_ret = (BitStream*)  new BitStream();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"BitStream");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_new00_local
static int tolua_network_BitStream_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BitStream* tolua_ret = (BitStream*)  new BitStream();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"BitStream");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_delete00
static int tolua_network_BitStream_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_clear00
static int tolua_network_BitStream_clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'",NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readString of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_readString00
static int tolua_network_BitStream_readString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readString'",NULL);
#endif
  {
   char* tolua_ret = (char*)  self->readString();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readInt of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_readInt00
static int tolua_network_BitStream_readInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readInt'",NULL);
#endif
  {
   int tolua_ret = (int)  self->readInt();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readBool of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_readBool00
static int tolua_network_BitStream_readBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readBool'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->readBool();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeInt of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_writeInt00
static int tolua_network_BitStream_writeInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeInt'",NULL);
#endif
  {
   self->writeInt(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeString of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_writeString00
static int tolua_network_BitStream_writeString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
  const char* s = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeString'",NULL);
#endif
  {
   self->writeString(s);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeString of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_writeString01
static int tolua_network_BitStream_writeString01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
  const std::string s = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeString'",NULL);
#endif
  {
   self->writeString(s);
   tolua_pushcppstring(tolua_S,(const char*)s);
  }
 }
 return 1;
tolua_lerror:
 return tolua_network_BitStream_writeString00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeBool of class  BitStream */
#ifndef TOLUA_DISABLE_tolua_network_BitStream_writeBool00
static int tolua_network_BitStream_writeBool00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BitStream",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BitStream* self = (BitStream*)  tolua_tousertype(tolua_S,1,0);
  bool b = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeBool'",NULL);
#endif
  {
   self->writeBool(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeBool'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  NetConnection */
#ifndef TOLUA_DISABLE_tolua_network_NetConnection_new00
static int tolua_network_NetConnection_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"NetConnection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   NetConnection* tolua_ret = (NetConnection*)  new NetConnection();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"NetConnection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  NetConnection */
#ifndef TOLUA_DISABLE_tolua_network_NetConnection_new00_local
static int tolua_network_NetConnection_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"NetConnection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   NetConnection* tolua_ret = (NetConnection*)  new NetConnection();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"NetConnection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  NetConnection */
#ifndef TOLUA_DISABLE_tolua_network_NetConnection_delete00
static int tolua_network_NetConnection_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NetConnection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NetConnection* self = (NetConnection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  NetConnection */
#ifndef TOLUA_DISABLE_tolua_network_NetConnection_create00
static int tolua_network_NetConnection_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NetConnection",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NetConnection* self = (NetConnection*)  tolua_tousertype(tolua_S,1,0);
  int port = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(port);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disconnect of class  NetConnection */
#ifndef TOLUA_DISABLE_tolua_network_NetConnection_disconnect00
static int tolua_network_NetConnection_disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NetConnection",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NetConnection* self = (NetConnection*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disconnect'",NULL);
#endif
  {
   self->disconnect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send of class  NetConnection */
#ifndef TOLUA_DISABLE_tolua_network_NetConnection_send00
static int tolua_network_NetConnection_send00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"NetConnection",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  NetConnection* self = (NetConnection*)  tolua_tousertype(tolua_S,1,0);
  BitStream* stream = ((BitStream*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send'",NULL);
#endif
  {
   self->send(stream);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Player */
#ifndef TOLUA_DISABLE_tolua_network_Player_new00
static int tolua_network_Player_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Player",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Player* tolua_ret = (Player*)  new Player();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Player");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Player */
#ifndef TOLUA_DISABLE_tolua_network_Player_new00_local
static int tolua_network_Player_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Player",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Player* tolua_ret = (Player*)  new Player();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Player");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: client of class  Player */
#ifndef TOLUA_DISABLE_tolua_get_Player_client
static int tolua_get_Player_client(lua_State* tolua_S)
{
  Player* self = (Player*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'client'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->client);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: client of class  Player */
#ifndef TOLUA_DISABLE_tolua_set_Player_client
static int tolua_set_Player_client(lua_State* tolua_S)
{
  Player* self = (Player*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'client'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->client = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: controler of class  Player */
#ifndef TOLUA_DISABLE_tolua_get_Player_controler_ptr
static int tolua_get_Player_controler_ptr(lua_State* tolua_S)
{
  Player* self = (Player*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'controler'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)self->controler,"Object");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: controler of class  Player */
#ifndef TOLUA_DISABLE_tolua_set_Player_controler_ptr
static int tolua_set_Player_controler_ptr(lua_State* tolua_S)
{
  Player* self = (Player*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'controler'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Object",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->controler = ((Object*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: name of class  Player */
#ifndef TOLUA_DISABLE_tolua_get_Player_name
static int tolua_get_Player_name(lua_State* tolua_S)
{
  Player* self = (Player*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: name of class  Player */
#ifndef TOLUA_DISABLE_tolua_set_Player_name
static int tolua_set_Player_name(lua_State* tolua_S)
{
  Player* self = (Player*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'name'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->name = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_create00
static int tolua_network_Process_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_destroy00
static int tolua_network_Process_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destroy'",NULL);
#endif
  {
   self->destroy();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getConnection of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_getConnection00
static int tolua_network_Process_getConnection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getConnection'",NULL);
#endif
  {
   NetConnection* tolua_ret = (NetConnection*)  self->getConnection();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"NetConnection");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getConnection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getSceneGraph of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_getSceneGraph00
static int tolua_network_Process_getSceneGraph00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getSceneGraph'",NULL);
#endif
  {
   SceneGraph& tolua_ret = (SceneGraph&)  self->getSceneGraph();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"SceneGraph");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getSceneGraph'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getActionMap of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_getActionMap00
static int tolua_network_Process_getActionMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActionMap'",NULL);
#endif
  {
   ActionMap* tolua_ret = (ActionMap*)  self->getActionMap();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ActionMap");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActionMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setActionMap of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_setActionMap00
static int tolua_network_Process_setActionMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ActionMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
  ActionMap* map = ((ActionMap*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActionMap'",NULL);
#endif
  {
   self->setActionMap(map);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActionMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendScriptEvent of class  Process */
#ifndef TOLUA_DISABLE_tolua_network_Process_sendScriptEvent00
static int tolua_network_Process_sendScriptEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Process",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"BitStream",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Process* self = (Process*)  tolua_tousertype(tolua_S,1,0);
  BitStream* stream = ((BitStream*)  tolua_tousertype(tolua_S,2,0));
  int client = ((int)  tolua_tonumber(tolua_S,3,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendScriptEvent'",NULL);
#endif
  {
   self->sendScriptEvent(stream,client);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendScriptEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addPlayer of class  Server */
#ifndef TOLUA_DISABLE_tolua_network_Server_addPlayer00
static int tolua_network_Server_addPlayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Player",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  int client = ((int)  tolua_tonumber(tolua_S,2,0));
  Player* player = ((Player*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addPlayer'",NULL);
#endif
  {
   self->addPlayer(client,player);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addPlayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendScriptEventToAllClients of class  Server */
#ifndef TOLUA_DISABLE_tolua_network_Server_sendScriptEventToAllClients00
static int tolua_network_Server_sendScriptEventToAllClients00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Server",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"BitStream",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Server* self = (Server*)  tolua_tousertype(tolua_S,1,0);
  BitStream* stream = ((BitStream*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendScriptEventToAllClients'",NULL);
#endif
  {
   self->sendScriptEventToAllClients(stream);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendScriptEventToAllClients'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: connect of class  Client */
#ifndef TOLUA_DISABLE_tolua_network_Client_connect00
static int tolua_network_Client_connect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Client",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Client* self = (Client*)  tolua_tousertype(tolua_S,1,0);
  const char* serverName = ((const char*)  tolua_tostring(tolua_S,2,0));
  int port = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'connect'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->connect(serverName,port,name);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'connect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: disconnect of class  Client */
#ifndef TOLUA_DISABLE_tolua_network_Client_disconnect00
static int tolua_network_Client_disconnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Client* self = (Client*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'disconnect'",NULL);
#endif
  {
   self->disconnect();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disconnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_network_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"INVALID_CLIENTID",INVALID_CLIENTID);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"BitStream","BitStream","",tolua_collect_BitStream);
  #else
  tolua_cclass(tolua_S,"BitStream","BitStream","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"BitStream");
   tolua_function(tolua_S,"new",tolua_network_BitStream_new00);
   tolua_function(tolua_S,"new_local",tolua_network_BitStream_new00_local);
   tolua_function(tolua_S,".call",tolua_network_BitStream_new00_local);
   tolua_function(tolua_S,"delete",tolua_network_BitStream_delete00);
   tolua_function(tolua_S,"clear",tolua_network_BitStream_clear00);
   tolua_function(tolua_S,"readString",tolua_network_BitStream_readString00);
   tolua_function(tolua_S,"readInt",tolua_network_BitStream_readInt00);
   tolua_function(tolua_S,"readBool",tolua_network_BitStream_readBool00);
   tolua_function(tolua_S,"writeInt",tolua_network_BitStream_writeInt00);
   tolua_function(tolua_S,"writeString",tolua_network_BitStream_writeString00);
   tolua_function(tolua_S,"writeString",tolua_network_BitStream_writeString01);
   tolua_function(tolua_S,"writeBool",tolua_network_BitStream_writeBool00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"NetConnection","NetConnection","",tolua_collect_NetConnection);
  #else
  tolua_cclass(tolua_S,"NetConnection","NetConnection","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"NetConnection");
   tolua_function(tolua_S,"new",tolua_network_NetConnection_new00);
   tolua_function(tolua_S,"new_local",tolua_network_NetConnection_new00_local);
   tolua_function(tolua_S,".call",tolua_network_NetConnection_new00_local);
   tolua_function(tolua_S,"delete",tolua_network_NetConnection_delete00);
   tolua_function(tolua_S,"create",tolua_network_NetConnection_create00);
   tolua_function(tolua_S,"disconnect",tolua_network_NetConnection_disconnect00);
   tolua_function(tolua_S,"send",tolua_network_NetConnection_send00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Player","Player","",tolua_collect_Player);
  #else
  tolua_cclass(tolua_S,"Player","Player","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Player");
   tolua_function(tolua_S,"new",tolua_network_Player_new00);
   tolua_function(tolua_S,"new_local",tolua_network_Player_new00_local);
   tolua_function(tolua_S,".call",tolua_network_Player_new00_local);
   tolua_variable(tolua_S,"client",tolua_get_Player_client,tolua_set_Player_client);
   tolua_variable(tolua_S,"controler",tolua_get_Player_controler_ptr,tolua_set_Player_controler_ptr);
   tolua_variable(tolua_S,"name",tolua_get_Player_name,tolua_set_Player_name);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Process","Process","",NULL);
  tolua_beginmodule(tolua_S,"Process");
   tolua_function(tolua_S,"create",tolua_network_Process_create00);
   tolua_function(tolua_S,"destroy",tolua_network_Process_destroy00);
   tolua_function(tolua_S,"getConnection",tolua_network_Process_getConnection00);
   tolua_function(tolua_S,"getSceneGraph",tolua_network_Process_getSceneGraph00);
   tolua_function(tolua_S,"getActionMap",tolua_network_Process_getActionMap00);
   tolua_function(tolua_S,"setActionMap",tolua_network_Process_setActionMap00);
   tolua_function(tolua_S,"sendScriptEvent",tolua_network_Process_sendScriptEvent00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Server","Server","Process",NULL);
  tolua_beginmodule(tolua_S,"Server");
   tolua_function(tolua_S,"addPlayer",tolua_network_Server_addPlayer00);
   tolua_function(tolua_S,"sendScriptEventToAllClients",tolua_network_Server_sendScriptEventToAllClients00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Client","Client","Process",NULL);
  tolua_beginmodule(tolua_S,"Client");
   tolua_function(tolua_S,"connect",tolua_network_Client_connect00);
   tolua_function(tolua_S,"disconnect",tolua_network_Client_disconnect00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_network (lua_State* tolua_S) {
 return tolua_network_open(tolua_S);
};
#endif

