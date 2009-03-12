/*
** Lua binding: editor
** Generated automatically by tolua++-1.0.92 on 12/29/08 11:10:27.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_editor_open (lua_State* tolua_S);

#include "world/world.h"
#include "gui/guihelpers.h"
#include "gui/guiimagelistbox.h"
#include "gui/guitileeditor.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_GuiTileEditor (lua_State* tolua_S)
{
 GuiTileEditor* self = (GuiTileEditor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiPoint (lua_State* tolua_S)
{
 GuiPoint* self = (GuiPoint*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiImageListBox (lua_State* tolua_S)
{
 GuiImageListBox* self = (GuiImageListBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"World");
 tolua_usertype(tolua_S,"GuiImageListBox");
 tolua_usertype(tolua_S,"GuiControl");
 tolua_usertype(tolua_S,"GuiPoint");
 tolua_usertype(tolua_S,"Bound");
 tolua_usertype(tolua_S,"GuiTileEditor");
 tolua_usertype(tolua_S,"GuiListBox");
}

/* method: new of class  GuiImageListBox */
#ifndef TOLUA_DISABLE_tolua_editor_GuiImageListBox_new00
static int tolua_editor_GuiImageListBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiImageListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiImageListBox* tolua_ret = (GuiImageListBox*)  new GuiImageListBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiImageListBox");
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

/* method: new_local of class  GuiImageListBox */
#ifndef TOLUA_DISABLE_tolua_editor_GuiImageListBox_new00_local
static int tolua_editor_GuiImageListBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiImageListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiImageListBox* tolua_ret = (GuiImageListBox*)  new GuiImageListBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiImageListBox");
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

/* method: getActiveLayer of class  GuiImageListBox */
#ifndef TOLUA_DISABLE_tolua_editor_GuiImageListBox_getActiveLayer00
static int tolua_editor_GuiImageListBox_getActiveLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiImageListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiImageListBox* self = (const GuiImageListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActiveLayer'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getActiveLayer();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActiveLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setActiveLayer of class  GuiImageListBox */
#ifndef TOLUA_DISABLE_tolua_editor_GuiImageListBox_setActiveLayer00
static int tolua_editor_GuiImageListBox_setActiveLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImageListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImageListBox* self = (GuiImageListBox*)  tolua_tousertype(tolua_S,1,0);
  int activeLayer = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActiveLayer'",NULL);
#endif
  {
   self->setActiveLayer(activeLayer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActiveLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: world of class  GuiImageListBox */
#ifndef TOLUA_DISABLE_tolua_editor_GuiImageListBox_world00
static int tolua_editor_GuiImageListBox_world00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImageListBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImageListBox* self = (GuiImageListBox*)  tolua_tousertype(tolua_S,1,0);
  const World* pworld = ((const World*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'world'",NULL);
#endif
  {
   self->world(pworld);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'world'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_new00
static int tolua_editor_GuiTileEditor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiTileEditor* tolua_ret = (GuiTileEditor*)  new GuiTileEditor();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiTileEditor");
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

/* method: new_local of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_new00_local
static int tolua_editor_GuiTileEditor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiTileEditor* tolua_ret = (GuiTileEditor*)  new GuiTileEditor();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiTileEditor");
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

/* method: getActiveLayer of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_getActiveLayer00
static int tolua_editor_GuiTileEditor_getActiveLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiTileEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiTileEditor* self = (const GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActiveLayer'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getActiveLayer();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActiveLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setActiveLayer of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_setActiveLayer00
static int tolua_editor_GuiTileEditor_setActiveLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiTileEditor* self = (GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
  int activeLayer = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActiveLayer'",NULL);
#endif
  {
   self->setActiveLayer(activeLayer);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActiveLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasWorld of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_hasWorld00
static int tolua_editor_GuiTileEditor_hasWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiTileEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiTileEditor* self = (const GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasWorld'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasWorld();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: world of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_world00
static int tolua_editor_GuiTileEditor_world00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiTileEditor* self = (GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'world'",NULL);
#endif
  {
   World& tolua_ret = (World&)  self->world();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"World");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'world'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: world of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_world01
static int tolua_editor_GuiTileEditor_world01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  GuiTileEditor* self = (GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
  World* pworld = ((World*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'world'",NULL);
#endif
  {
   self->world(pworld);
  }
 }
 return 0;
tolua_lerror:
 return tolua_editor_GuiTileEditor_world00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: PointToTile of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_PointToTile00
static int tolua_editor_GuiTileEditor_PointToTile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiTileEditor* self = (GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
  const GuiPoint* point = ((const GuiPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PointToTile'",NULL);
#endif
  {
   GuiPoint tolua_ret = (GuiPoint)  self->PointToTile(*point);
   {
#ifdef __cplusplus
    void* tolua_obj = new GuiPoint(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"GuiPoint");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(GuiPoint));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"GuiPoint");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PointToTile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showAll of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_showAll00
static int tolua_editor_GuiTileEditor_showAll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiTileEditor* self = (GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
  bool all = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showAll'",NULL);
#endif
  {
   self->showAll(all);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showAll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pickBound of class  GuiTileEditor */
#ifndef TOLUA_DISABLE_tolua_editor_GuiTileEditor_pickBound00
static int tolua_editor_GuiTileEditor_pickBound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiTileEditor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiTileEditor* self = (GuiTileEditor*)  tolua_tousertype(tolua_S,1,0);
  const GuiPoint* point = ((const GuiPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pickBound'",NULL);
#endif
  {
   Bound* tolua_ret = (Bound*)  self->pickBound(*point);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pickBound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_editor_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiImageListBox","GuiImageListBox","GuiListBox",tolua_collect_GuiImageListBox);
  #else
  tolua_cclass(tolua_S,"GuiImageListBox","GuiImageListBox","GuiListBox",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiImageListBox");
   tolua_function(tolua_S,"new",tolua_editor_GuiImageListBox_new00);
   tolua_function(tolua_S,"new_local",tolua_editor_GuiImageListBox_new00_local);
   tolua_function(tolua_S,".call",tolua_editor_GuiImageListBox_new00_local);
   tolua_function(tolua_S,"getActiveLayer",tolua_editor_GuiImageListBox_getActiveLayer00);
   tolua_function(tolua_S,"setActiveLayer",tolua_editor_GuiImageListBox_setActiveLayer00);
   tolua_function(tolua_S,"world",tolua_editor_GuiImageListBox_world00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiTileEditor","GuiTileEditor","GuiControl",tolua_collect_GuiTileEditor);
  #else
  tolua_cclass(tolua_S,"GuiTileEditor","GuiTileEditor","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiTileEditor");
   tolua_function(tolua_S,"new",tolua_editor_GuiTileEditor_new00);
   tolua_function(tolua_S,"new_local",tolua_editor_GuiTileEditor_new00_local);
   tolua_function(tolua_S,".call",tolua_editor_GuiTileEditor_new00_local);
   tolua_function(tolua_S,"getActiveLayer",tolua_editor_GuiTileEditor_getActiveLayer00);
   tolua_function(tolua_S,"setActiveLayer",tolua_editor_GuiTileEditor_setActiveLayer00);
   tolua_function(tolua_S,"hasWorld",tolua_editor_GuiTileEditor_hasWorld00);
   tolua_function(tolua_S,"world",tolua_editor_GuiTileEditor_world00);
   tolua_function(tolua_S,"world",tolua_editor_GuiTileEditor_world01);
   tolua_function(tolua_S,"PointToTile",tolua_editor_GuiTileEditor_PointToTile00);
   tolua_function(tolua_S,"showAll",tolua_editor_GuiTileEditor_showAll00);
   tolua_function(tolua_S,"pickBound",tolua_editor_GuiTileEditor_pickBound00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_editor (lua_State* tolua_S) {
 return tolua_editor_open(tolua_S);
};
#endif

