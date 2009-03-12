/*
** Lua binding: gui
** Generated automatically by tolua++-1.0.92 on 12/29/08 11:10:27.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_gui_open (lua_State* tolua_S);

#include "gui/guicontrol.h"
#include "gui/guiheadercontrol.h"
#include "gui/guibutton.h"
#include "gui/guiimagebutton.h"
#include "gui/guicanvas.h"
#include "gui/guidialog.h"
#include "gui/guigamewnd.h"
#include "gui/guiedit.h"
#include "gui/guistatic.h"
#include "gui/guilistbox.h"
#include "gui/guicheckbox.h"
#include "gui/guicombobox.h"
#include "gui/guiscriptwnd.h"
#include "gui/guiimage.h"
#include "gui/guifocus.h"
#include "gui/guimanager.h"
#include "gui/guimenu.h"
#include "gui/guieventhandlerdefinition.h"
#include "gui/guieventhandlerdefinitions.h"
#include "gui/guifilelistbox.h"
#include "gui/guitreebox.h"
#include <SDL/SDL.h>

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_GuiGameWnd (lua_State* tolua_S)
{
 GuiGameWnd* self = (GuiGameWnd*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiRect (lua_State* tolua_S)
{
 GuiRect* self = (GuiRect*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiPoint (lua_State* tolua_S)
{
 GuiPoint* self = (GuiPoint*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiTreeBox (lua_State* tolua_S)
{
 GuiTreeBox* self = (GuiTreeBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiColor (lua_State* tolua_S)
{
 GuiColor* self = (GuiColor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiComboBox (lua_State* tolua_S)
{
 GuiComboBox* self = (GuiComboBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiCheckBox (lua_State* tolua_S)
{
 GuiCheckBox* self = (GuiCheckBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiButton (lua_State* tolua_S)
{
 GuiButton* self = (GuiButton*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiDialog (lua_State* tolua_S)
{
 GuiDialog* self = (GuiDialog*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiFileListBox (lua_State* tolua_S)
{
 GuiFileListBox* self = (GuiFileListBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiWindow (lua_State* tolua_S)
{
 GuiWindow* self = (GuiWindow*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiListBox (lua_State* tolua_S)
{
 GuiListBox* self = (GuiListBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiHeaderCtrl (lua_State* tolua_S)
{
 GuiHeaderCtrl* self = (GuiHeaderCtrl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiStatic (lua_State* tolua_S)
{
 GuiStatic* self = (GuiStatic*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiControl (lua_State* tolua_S)
{
 GuiControl* self = (GuiControl*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiEditBox (lua_State* tolua_S)
{
 GuiEditBox* self = (GuiEditBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiScriptWnd (lua_State* tolua_S)
{
 GuiScriptWnd* self = (GuiScriptWnd*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiColorComboBox (lua_State* tolua_S)
{
 GuiColorComboBox* self = (GuiColorComboBox*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiImageButton (lua_State* tolua_S)
{
 GuiImageButton* self = (GuiImageButton*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiMovie (lua_State* tolua_S)
{
 GuiMovie* self = (GuiMovie*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_GuiImage (lua_State* tolua_S)
{
 GuiImage* self = (GuiImage*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GuiGameWnd");
 tolua_usertype(tolua_S,"GuiFocus");
 tolua_usertype(tolua_S,"GuiRect");
 tolua_usertype(tolua_S,"GuiEventHandlerDefinitions");
 tolua_usertype(tolua_S,"GuiPoint");
 tolua_usertype(tolua_S,"GuiManager");
 tolua_usertype(tolua_S,"GuiTreeBox");
 tolua_usertype(tolua_S,"GuiMenu");
 tolua_usertype(tolua_S,"KeyMap");
 tolua_usertype(tolua_S,"GuiCanvas");
 tolua_usertype(tolua_S,"GuiColorComboBox");
 tolua_usertype(tolua_S,"GuiComboBox");
 tolua_usertype(tolua_S,"GuiFileListBox");
 tolua_usertype(tolua_S,"GuiListBox");
 tolua_usertype(tolua_S,"GuiHeaderCtrl");
 tolua_usertype(tolua_S,"GuiCheckBox");
 tolua_usertype(tolua_S,"GuiButton");
 tolua_usertype(tolua_S,"GuiControl");
 tolua_usertype(tolua_S,"GuiEditBox");
 tolua_usertype(tolua_S,"GuiWindow");
 tolua_usertype(tolua_S,"GuiMovie");
 tolua_usertype(tolua_S,"GuiImage");
 tolua_usertype(tolua_S,"GuiStatic");
 tolua_usertype(tolua_S,"Vector");
 tolua_usertype(tolua_S,"GuiDialog");
 tolua_usertype(tolua_S,"GuiScriptWnd");
 tolua_usertype(tolua_S,"GuiEventHandlerDefinition");
 tolua_usertype(tolua_S,"GuiImageButton");
 tolua_usertype(tolua_S,"GuiColor");
 tolua_usertype(tolua_S,"GuiWnd");
}

/* function: SDL_EnableKeyRepeat */
#ifndef TOLUA_DISABLE_tolua_gui_SDL_EnableKeyRepeat00
static int tolua_gui_SDL_EnableKeyRepeat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int delay = ((int)  tolua_tonumber(tolua_S,1,0));
  int interval = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   int tolua_ret = (int)  SDL_EnableKeyRepeat(delay,interval);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SDL_EnableKeyRepeat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: hasFocus of class  GuiFocus */
#ifndef TOLUA_DISABLE_tolua_gui_GuiFocus_hasFocus00
static int tolua_gui_GuiFocus_hasFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiFocus",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiFocus* self = (const GuiFocus*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasFocus'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->hasFocus();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'hasFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFocus of class  GuiFocus */
#ifndef TOLUA_DISABLE_tolua_gui_GuiFocus_getFocus00
static int tolua_gui_GuiFocus_getFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiFocus",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiFocus* self = (GuiFocus*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFocus'",NULL);
#endif
  {
   GuiWnd& tolua_ret = (GuiWnd&)  self->getFocus();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"GuiWnd");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_gui_GuiPoint_new00
static int tolua_gui_GuiPoint_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiPoint* tolua_ret = (GuiPoint*)  new GuiPoint();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiPoint");
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

/* method: new_local of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_gui_GuiPoint_new00_local
static int tolua_gui_GuiPoint_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiPoint* tolua_ret = (GuiPoint*)  new GuiPoint();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiPoint");
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

/* method: new of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_gui_GuiPoint_new01
static int tolua_gui_GuiPoint_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiPoint",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int _x = ((int)  tolua_tonumber(tolua_S,2,0));
  int _y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   GuiPoint* tolua_ret = (GuiPoint*)  new GuiPoint(_x,_y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiPoint");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiPoint_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_gui_GuiPoint_new01_local
static int tolua_gui_GuiPoint_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiPoint",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int _x = ((int)  tolua_tonumber(tolua_S,2,0));
  int _y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   GuiPoint* tolua_ret = (GuiPoint*)  new GuiPoint(_x,_y);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiPoint");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiPoint_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_gui_GuiPoint_new02
static int tolua_gui_GuiPoint_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiPoint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const GuiPoint* other = ((const GuiPoint*)  tolua_tousertype(tolua_S,2,0));
  {
   GuiPoint* tolua_ret = (GuiPoint*)  new GuiPoint(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiPoint");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiPoint_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_gui_GuiPoint_new02_local
static int tolua_gui_GuiPoint_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiPoint",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const GuiPoint* other = ((const GuiPoint*)  tolua_tousertype(tolua_S,2,0));
  {
   GuiPoint* tolua_ret = (GuiPoint*)  new GuiPoint(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiPoint");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiPoint_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_get_GuiPoint_x
static int tolua_get_GuiPoint_x(lua_State* tolua_S)
{
  GuiPoint* self = (GuiPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_set_GuiPoint_x
static int tolua_set_GuiPoint_x(lua_State* tolua_S)
{
  GuiPoint* self = (GuiPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_get_GuiPoint_y
static int tolua_get_GuiPoint_y(lua_State* tolua_S)
{
  GuiPoint* self = (GuiPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  GuiPoint */
#ifndef TOLUA_DISABLE_tolua_set_GuiPoint_y
static int tolua_set_GuiPoint_y(lua_State* tolua_S)
{
  GuiPoint* self = (GuiPoint*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColor_new00
static int tolua_gui_GuiColor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiColor* tolua_ret = (GuiColor*)  new GuiColor();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiColor");
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

/* method: new_local of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColor_new00_local
static int tolua_gui_GuiColor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiColor* tolua_ret = (GuiColor*)  new GuiColor();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiColor");
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

/* method: new of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColor_new01
static int tolua_gui_GuiColor_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int _r = ((int)  tolua_tonumber(tolua_S,2,0));
  int _g = ((int)  tolua_tonumber(tolua_S,3,0));
  int _b = ((int)  tolua_tonumber(tolua_S,4,0));
  int _a = ((int)  tolua_tonumber(tolua_S,5,255));
  {
   GuiColor* tolua_ret = (GuiColor*)  new GuiColor(_r,_g,_b,_a);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiColor");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiColor_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColor_new01_local
static int tolua_gui_GuiColor_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int _r = ((int)  tolua_tonumber(tolua_S,2,0));
  int _g = ((int)  tolua_tonumber(tolua_S,3,0));
  int _b = ((int)  tolua_tonumber(tolua_S,4,0));
  int _a = ((int)  tolua_tonumber(tolua_S,5,255));
  {
   GuiColor* tolua_ret = (GuiColor*)  new GuiColor(_r,_g,_b,_a);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiColor");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiColor_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: r of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_get_GuiColor_r
static int tolua_get_GuiColor_r(lua_State* tolua_S)
{
  GuiColor* self = (GuiColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->r);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: r of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_set_GuiColor_r
static int tolua_set_GuiColor_r(lua_State* tolua_S)
{
  GuiColor* self = (GuiColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->r = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_get_GuiColor_g
static int tolua_get_GuiColor_g(lua_State* tolua_S)
{
  GuiColor* self = (GuiColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->g);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_set_GuiColor_g
static int tolua_set_GuiColor_g(lua_State* tolua_S)
{
  GuiColor* self = (GuiColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->g = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_get_GuiColor_b
static int tolua_get_GuiColor_b(lua_State* tolua_S)
{
  GuiColor* self = (GuiColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->b);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b of class  GuiColor */
#ifndef TOLUA_DISABLE_tolua_set_GuiColor_b
static int tolua_set_GuiColor_b(lua_State* tolua_S)
{
  GuiColor* self = (GuiColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_new00
static int tolua_gui_GuiRect_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiRect* tolua_ret = (GuiRect*)  new GuiRect();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiRect");
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

/* method: new_local of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_new00_local
static int tolua_gui_GuiRect_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiRect* tolua_ret = (GuiRect*)  new GuiRect();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiRect");
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

/* method: new of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_new01
static int tolua_gui_GuiRect_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiRect",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float l = ((float)  tolua_tonumber(tolua_S,2,0));
  float r = ((float)  tolua_tonumber(tolua_S,3,0));
  float t = ((float)  tolua_tonumber(tolua_S,4,0));
  float b = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   GuiRect* tolua_ret = (GuiRect*)  new GuiRect(l,r,t,b);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiRect");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiRect_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_new01_local
static int tolua_gui_GuiRect_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiRect",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float l = ((float)  tolua_tonumber(tolua_S,2,0));
  float r = ((float)  tolua_tonumber(tolua_S,3,0));
  float t = ((float)  tolua_tonumber(tolua_S,4,0));
  float b = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   GuiRect* tolua_ret = (GuiRect*)  new GuiRect(l,r,t,b);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiRect");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiRect_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_set00
static int tolua_gui_GuiRect_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiRect",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiRect* self = (GuiRect*)  tolua_tousertype(tolua_S,1,0);
  float l = ((float)  tolua_tonumber(tolua_S,2,0));
  float r = ((float)  tolua_tonumber(tolua_S,3,0));
  float t = ((float)  tolua_tonumber(tolua_S,4,0));
  float b = ((float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(l,r,t,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pointInRect of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_pointInRect00
static int tolua_gui_GuiRect_pointInRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiRect",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiRect* self = (GuiRect*)  tolua_tousertype(tolua_S,1,0);
  const GuiPoint* point = ((const GuiPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pointInRect'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->pointInRect(*point);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pointInRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: left of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_left00
static int tolua_gui_GuiRect_left00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiRect* self = (const GuiRect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'left'",NULL);
#endif
  {
   float tolua_ret = (float)  self->left();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'left'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: right of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_right00
static int tolua_gui_GuiRect_right00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiRect* self = (const GuiRect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'right'",NULL);
#endif
  {
   float tolua_ret = (float)  self->right();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'right'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: top of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_top00
static int tolua_gui_GuiRect_top00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiRect* self = (const GuiRect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'top'",NULL);
#endif
  {
   float tolua_ret = (float)  self->top();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'top'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: bottom of class  GuiRect */
#ifndef TOLUA_DISABLE_tolua_gui_GuiRect_bottom00
static int tolua_gui_GuiRect_bottom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiRect* self = (const GuiRect*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bottom'",NULL);
#endif
  {
   float tolua_ret = (float)  self->bottom();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bottom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  GuiEventHandlerDefinition */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEventHandlerDefinition_getName00
static int tolua_gui_GuiEventHandlerDefinition_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiEventHandlerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiEventHandlerDefinition* self = (const GuiEventHandlerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCode of class  GuiEventHandlerDefinition */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEventHandlerDefinition_getCode00
static int tolua_gui_GuiEventHandlerDefinition_getCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiEventHandlerDefinition",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiEventHandlerDefinition* self = (const GuiEventHandlerDefinition*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCode'",NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getCode();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCode of class  GuiEventHandlerDefinition */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEventHandlerDefinition_setCode00
static int tolua_gui_GuiEventHandlerDefinition_setCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiEventHandlerDefinition",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiEventHandlerDefinition* self = (GuiEventHandlerDefinition*)  tolua_tousertype(tolua_S,1,0);
  const std::string code = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCode'",NULL);
#endif
  {
   self->setCode(code);
   tolua_pushcppstring(tolua_S,(const char*)code);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDefinition of class  GuiEventHandlerDefinitions */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEventHandlerDefinitions_getDefinition00
static int tolua_gui_GuiEventHandlerDefinitions_getDefinition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiEventHandlerDefinitions",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiEventHandlerDefinitions* self = (GuiEventHandlerDefinitions*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDefinition'",NULL);
#endif
  {
   GuiEventHandlerDefinition* tolua_ret = (GuiEventHandlerDefinition*)  self->getDefinition(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiEventHandlerDefinition");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDefinition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  GuiEventHandlerDefinitions */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEventHandlerDefinitions_size00
static int tolua_gui_GuiEventHandlerDefinitions_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiEventHandlerDefinitions",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiEventHandlerDefinitions* self = (const GuiEventHandlerDefinitions*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   int tolua_ret = (int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_create00
static int tolua_gui_GuiWnd_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const GuiRect",0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"GuiWnd",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  const GuiRect* rect = ((const GuiRect*)  tolua_tousertype(tolua_S,3,0));
  const char* caption = ((const char*)  tolua_tostring(tolua_S,4,""));
  int style = ((int)  tolua_tonumber(tolua_S,5,0));
  GuiWnd* parent = ((GuiWnd*)  tolua_tousertype(tolua_S,6,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   self->create(id,*rect,caption,style,parent);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addChild of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_addChild00
static int tolua_gui_GuiWnd_addChild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  GuiWnd* wnd = ((GuiWnd*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addChild'",NULL);
#endif
  {
   self->addChild(wnd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addChild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getId of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_getId00
static int tolua_gui_GuiWnd_getId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getId'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWindowRect of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_getWindowRect00
static int tolua_gui_GuiWnd_getWindowRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWindowRect'",NULL);
#endif
  {
   const GuiRect& tolua_ret = (const GuiRect&)  self->getWindowRect();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const GuiRect");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWindowRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveWindow of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_moveWindow00
static int tolua_gui_GuiWnd_moveWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveWindow'",NULL);
#endif
  {
   self->moveWindow(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWindowPos of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_setWindowPos00
static int tolua_gui_GuiWnd_setWindowPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWindowPos'",NULL);
#endif
  {
   self->setWindowPos(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWindowPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVisible of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_setVisible00
static int tolua_gui_GuiWnd_setVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  bool vis = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVisible'",NULL);
#endif
  {
   self->setVisible(vis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCaption of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_setCaption00
static int tolua_gui_GuiWnd_setCaption00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* caption = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCaption'",NULL);
#endif
  {
   self->setCaption(caption);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCaption'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBackgroundImage of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_setBackgroundImage00
static int tolua_gui_GuiWnd_setBackgroundImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* file = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBackgroundImage'",NULL);
#endif
  {
   self->setBackgroundImage(file);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBackgroundImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFocus of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_setFocus00
static int tolua_gui_GuiWnd_setFocus00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFocus'",NULL);
#endif
  {
   self->setFocus();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFocus'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: center of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_center00
static int tolua_gui_GuiWnd_center00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'center'",NULL);
#endif
  {
   self->center();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'center'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clientToWindow of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_clientToWindow00
static int tolua_gui_GuiWnd_clientToWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiWnd* self = (const GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  GuiRect* rect = ((GuiRect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clientToWindow'",NULL);
#endif
  {
   self->clientToWindow(*rect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clientToWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: windowToClient of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_windowToClient00
static int tolua_gui_GuiWnd_windowToClient00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiWnd* self = (const GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  GuiRect* rect = ((GuiRect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToClient'",NULL);
#endif
  {
   self->windowToClient(*rect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'windowToClient'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: windowToClient of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_windowToClient01
static int tolua_gui_GuiWnd_windowToClient01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiPoint",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  GuiPoint* point = ((GuiPoint*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'windowToClient'",NULL);
#endif
  {
   self->windowToClient(*point);
  }
 }
 return 0;
tolua_lerror:
 return tolua_gui_GuiWnd_windowToClient00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getParent of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_getParent00
static int tolua_gui_GuiWnd_getParent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiWnd* self = (const GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getParent'",NULL);
#endif
  {
   GuiWnd* tolua_ret = (GuiWnd*)  self->getParent();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiWnd");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getParent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemById of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_getItemById00
static int tolua_gui_GuiWnd_getItemById00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemById'",NULL);
#endif
  {
   GuiWnd* tolua_ret = (GuiWnd*)  self->getItemById(id);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiWnd");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemById'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findChildByName of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_findChildByName00
static int tolua_gui_GuiWnd_findChildByName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findChildByName'",NULL);
#endif
  {
   GuiWnd* tolua_ret = (GuiWnd*)  self->findChildByName(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiWnd");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findChildByName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCaption of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_getCaption00
static int tolua_gui_GuiWnd_getCaption00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCaption'",NULL);
#endif
  {
   std::string tolua_ret = (std::string)  self->getCaption();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCaption'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEventHandlerDefinitions of class  GuiWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWnd_getEventHandlerDefinitions00
static int tolua_gui_GuiWnd_getEventHandlerDefinitions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWnd* self = (GuiWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEventHandlerDefinitions'",NULL);
#endif
  {
   GuiEventHandlerDefinitions& tolua_ret = (GuiEventHandlerDefinitions&)  self->getEventHandlerDefinitions();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"GuiEventHandlerDefinitions");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEventHandlerDefinitions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiWindow */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWindow_new00
static int tolua_gui_GuiWindow_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiWindow* tolua_ret = (GuiWindow*)  new GuiWindow();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiWindow");
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

/* method: new_local of class  GuiWindow */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWindow_new00_local
static int tolua_gui_GuiWindow_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiWindow",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiWindow* tolua_ret = (GuiWindow*)  new GuiWindow();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiWindow");
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

/* method: setActive of class  GuiWindow */
#ifndef TOLUA_DISABLE_tolua_gui_GuiWindow_setActive00
static int tolua_gui_GuiWindow_setActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiWindow",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiWindow* self = (GuiWindow*)  tolua_tousertype(tolua_S,1,0);
  bool active = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActive'",NULL);
#endif
  {
   self->setActive(active);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiScriptWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiScriptWnd_new00
static int tolua_gui_GuiScriptWnd_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiScriptWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiScriptWnd* tolua_ret = (GuiScriptWnd*)  new GuiScriptWnd();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiScriptWnd");
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

/* method: new_local of class  GuiScriptWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiScriptWnd_new00_local
static int tolua_gui_GuiScriptWnd_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiScriptWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiScriptWnd* tolua_ret = (GuiScriptWnd*)  new GuiScriptWnd();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiScriptWnd");
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

/* method: delete of class  GuiScriptWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiScriptWnd_delete00
static int tolua_gui_GuiScriptWnd_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiScriptWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiScriptWnd* self = (GuiScriptWnd*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setMouseMoveFnc of class  GuiScriptWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiScriptWnd_setMouseMoveFnc00
static int tolua_gui_GuiScriptWnd_setMouseMoveFnc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiScriptWnd* self = (GuiScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* fnc = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMouseMoveFnc'",NULL);
#endif
  {
   self->setMouseMoveFnc(fnc);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMouseMoveFnc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLButtonDownFnc of class  GuiScriptWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiScriptWnd_setLButtonDownFnc00
static int tolua_gui_GuiScriptWnd_setLButtonDownFnc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiScriptWnd* self = (GuiScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* fnc = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLButtonDownFnc'",NULL);
#endif
  {
   self->setLButtonDownFnc(fnc);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLButtonDownFnc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setKeyDownFnc of class  GuiScriptWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiScriptWnd_setKeyDownFnc00
static int tolua_gui_GuiScriptWnd_setKeyDownFnc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiScriptWnd* self = (GuiScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* fnc = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setKeyDownFnc'",NULL);
#endif
  {
   self->setKeyDownFnc(fnc);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setKeyDownFnc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiGameWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiGameWnd_new00
static int tolua_gui_GuiGameWnd_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiGameWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiGameWnd* tolua_ret = (GuiGameWnd*)  new GuiGameWnd();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiGameWnd");
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

/* method: new_local of class  GuiGameWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiGameWnd_new00_local
static int tolua_gui_GuiGameWnd_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiGameWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiGameWnd* tolua_ret = (GuiGameWnd*)  new GuiGameWnd();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiGameWnd");
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

/* method: pushMap of class  GuiGameWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiGameWnd_pushMap00
static int tolua_gui_GuiGameWnd_pushMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiGameWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"KeyMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiGameWnd* self = (GuiGameWnd*)  tolua_tousertype(tolua_S,1,0);
  KeyMap* map = ((KeyMap*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pushMap'",NULL);
#endif
  {
   self->pushMap(map);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: popMap of class  GuiGameWnd */
#ifndef TOLUA_DISABLE_tolua_gui_GuiGameWnd_popMap00
static int tolua_gui_GuiGameWnd_popMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiGameWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiGameWnd* self = (GuiGameWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popMap'",NULL);
#endif
  {
   self->popMap();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'popMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiControl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiControl_new00
static int tolua_gui_GuiControl_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiControl* tolua_ret = (GuiControl*)  new GuiControl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiControl");
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

/* method: new_local of class  GuiControl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiControl_new00_local
static int tolua_gui_GuiControl_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiControl* tolua_ret = (GuiControl*)  new GuiControl();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiControl");
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

/* method: delete of class  GuiControl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiControl_delete00
static int tolua_gui_GuiControl_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiControl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiControl* self = (GuiControl*)  tolua_tousertype(tolua_S,1,0);
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

/* method: setCallback of class  GuiControl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiControl_setCallback00
static int tolua_gui_GuiControl_setCallback00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiControl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiControl* self = (GuiControl*)  tolua_tousertype(tolua_S,1,0);
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* fnc = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCallback'",NULL);
#endif
  {
   self->setCallback(type,fnc);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCallback'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiDialog */
#ifndef TOLUA_DISABLE_tolua_gui_GuiDialog_new00
static int tolua_gui_GuiDialog_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiDialog",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiDialog* tolua_ret = (GuiDialog*)  new GuiDialog();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiDialog");
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

/* method: new_local of class  GuiDialog */
#ifndef TOLUA_DISABLE_tolua_gui_GuiDialog_new00_local
static int tolua_gui_GuiDialog_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiDialog",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiDialog* tolua_ret = (GuiDialog*)  new GuiDialog();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiDialog");
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

/* method: doModal of class  GuiDialog */
#ifndef TOLUA_DISABLE_tolua_gui_GuiDialog_doModal00
static int tolua_gui_GuiDialog_doModal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiDialog",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiDialog* self = (GuiDialog*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doModal'",NULL);
#endif
  {
   int tolua_ret = (int)  self->doModal();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doModal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: endModal of class  GuiDialog */
#ifndef TOLUA_DISABLE_tolua_gui_GuiDialog_endModal00
static int tolua_gui_GuiDialog_endModal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiDialog",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiDialog* self = (GuiDialog*)  tolua_tousertype(tolua_S,1,0);
  int retValue = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'endModal'",NULL);
#endif
  {
   self->endModal(retValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'endModal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiStatic */
#ifndef TOLUA_DISABLE_tolua_gui_GuiStatic_new00
static int tolua_gui_GuiStatic_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiStatic",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiStatic* tolua_ret = (GuiStatic*)  new GuiStatic();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiStatic");
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

/* method: new_local of class  GuiStatic */
#ifndef TOLUA_DISABLE_tolua_gui_GuiStatic_new00_local
static int tolua_gui_GuiStatic_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiStatic",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiStatic* tolua_ret = (GuiStatic*)  new GuiStatic();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiStatic");
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

/* method: new of class  GuiStatic */
#ifndef TOLUA_DISABLE_tolua_gui_GuiStatic_new01
static int tolua_gui_GuiStatic_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiStatic",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"GuiRect",0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"GuiWnd",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  GuiRect* rect = ((GuiRect*)  tolua_tousertype(tolua_S,3,0));
  const char* caption = ((const char*)  tolua_tostring(tolua_S,4,""));
  int style = ((int)  tolua_tonumber(tolua_S,5,0));
  GuiWnd* parent = ((GuiWnd*)  tolua_tousertype(tolua_S,6,NULL));
  {
   GuiStatic* tolua_ret = (GuiStatic*)  new GuiStatic(id,*rect,caption,style,parent);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiStatic");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiStatic_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  GuiStatic */
#ifndef TOLUA_DISABLE_tolua_gui_GuiStatic_new01_local
static int tolua_gui_GuiStatic_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiStatic",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"GuiRect",0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"GuiWnd",1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  GuiRect* rect = ((GuiRect*)  tolua_tousertype(tolua_S,3,0));
  const char* caption = ((const char*)  tolua_tostring(tolua_S,4,""));
  int style = ((int)  tolua_tonumber(tolua_S,5,0));
  GuiWnd* parent = ((GuiWnd*)  tolua_tousertype(tolua_S,6,NULL));
  {
   GuiStatic* tolua_ret = (GuiStatic*)  new GuiStatic(id,*rect,caption,style,parent);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiStatic");
  }
 }
 return 1;
tolua_lerror:
 return tolua_gui_GuiStatic_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiButton_new00
static int tolua_gui_GuiButton_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiButton* tolua_ret = (GuiButton*)  new GuiButton();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiButton");
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

/* method: new_local of class  GuiButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiButton_new00_local
static int tolua_gui_GuiButton_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiButton* tolua_ret = (GuiButton*)  new GuiButton();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiButton");
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

/* method: setCommand of class  GuiButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiButton_setCommand00
static int tolua_gui_GuiButton_setCommand00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiButton",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiButton* self = (GuiButton*)  tolua_tousertype(tolua_S,1,0);
  const char* cmd = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCommand'",NULL);
#endif
  {
   self->setCommand(cmd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCommand'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiImageButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImageButton_new00
static int tolua_gui_GuiImageButton_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiImageButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiImageButton* tolua_ret = (GuiImageButton*)  new GuiImageButton();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiImageButton");
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

/* method: new_local of class  GuiImageButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImageButton_new00_local
static int tolua_gui_GuiImageButton_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiImageButton",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiImageButton* tolua_ret = (GuiImageButton*)  new GuiImageButton();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiImageButton");
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

/* method: setImage of class  GuiImageButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImageButton_setImage00
static int tolua_gui_GuiImageButton_setImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImageButton",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImageButton* self = (GuiImageButton*)  tolua_tousertype(tolua_S,1,0);
  const char* image = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setImage'",NULL);
#endif
  {
   self->setImage(image);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPressedImage of class  GuiImageButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImageButton_setPressedImage00
static int tolua_gui_GuiImageButton_setPressedImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImageButton",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImageButton* self = (GuiImageButton*)  tolua_tousertype(tolua_S,1,0);
  const std::string file = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPressedImage'",NULL);
#endif
  {
   self->setPressedImage(file);
   tolua_pushcppstring(tolua_S,(const char*)file);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPressedImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTextureCoordinates of class  GuiImageButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImageButton_setTextureCoordinates00
static int tolua_gui_GuiImageButton_setTextureCoordinates00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImageButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const GuiRect",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImageButton* self = (GuiImageButton*)  tolua_tousertype(tolua_S,1,0);
  const GuiRect* rect = ((const GuiRect*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTextureCoordinates'",NULL);
#endif
  {
   self->setTextureCoordinates(*rect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTextureCoordinates'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOffset of class  GuiImageButton */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImageButton_setOffset00
static int tolua_gui_GuiImageButton_setOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImageButton",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImageButton* self = (GuiImageButton*)  tolua_tousertype(tolua_S,1,0);
  const Vector* ofs = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOffset'",NULL);
#endif
  {
   self->setOffset(*ofs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiCheckBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCheckBox_new00
static int tolua_gui_GuiCheckBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiCheckBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiCheckBox* tolua_ret = (GuiCheckBox*)  new GuiCheckBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiCheckBox");
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

/* method: new_local of class  GuiCheckBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCheckBox_new00_local
static int tolua_gui_GuiCheckBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiCheckBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiCheckBox* tolua_ret = (GuiCheckBox*)  new GuiCheckBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiCheckBox");
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

/* method: isChecked of class  GuiCheckBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCheckBox_isChecked00
static int tolua_gui_GuiCheckBox_isChecked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const GuiCheckBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const GuiCheckBox* self = (const GuiCheckBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isChecked'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isChecked();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isChecked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiImage */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImage_new00
static int tolua_gui_GuiImage_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiImage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiImage* tolua_ret = (GuiImage*)  new GuiImage();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiImage");
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

/* method: new_local of class  GuiImage */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImage_new00_local
static int tolua_gui_GuiImage_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiImage",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiImage* tolua_ret = (GuiImage*)  new GuiImage();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiImage");
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

/* method: setImage of class  GuiImage */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImage_setImage00
static int tolua_gui_GuiImage_setImage00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImage",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImage* self = (GuiImage*)  tolua_tousertype(tolua_S,1,0);
  const char* image = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setImage'",NULL);
#endif
  {
   self->setImage(image);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setImage'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOffset of class  GuiImage */
#ifndef TOLUA_DISABLE_tolua_gui_GuiImage_setOffset00
static int tolua_gui_GuiImage_setOffset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiImage",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiImage* self = (GuiImage*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOffset'",NULL);
#endif
  {
   self->setOffset(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOffset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiMovie */
#ifndef TOLUA_DISABLE_tolua_gui_GuiMovie_new00
static int tolua_gui_GuiMovie_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiMovie",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiMovie* tolua_ret = (GuiMovie*)  new GuiMovie();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiMovie");
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

/* method: new_local of class  GuiMovie */
#ifndef TOLUA_DISABLE_tolua_gui_GuiMovie_new00_local
static int tolua_gui_GuiMovie_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiMovie",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiMovie* tolua_ret = (GuiMovie*)  new GuiMovie();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiMovie");
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

/* method: setMovie of class  GuiMovie */
#ifndef TOLUA_DISABLE_tolua_gui_GuiMovie_setMovie00
static int tolua_gui_GuiMovie_setMovie00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiMovie",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiMovie* self = (GuiMovie*)  tolua_tousertype(tolua_S,1,0);
  const char* movie = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMovie'",NULL);
#endif
  {
   self->setMovie(movie);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMovie'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiEditBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEditBox_new00
static int tolua_gui_GuiEditBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiEditBox* tolua_ret = (GuiEditBox*)  new GuiEditBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiEditBox");
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

/* method: new_local of class  GuiEditBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiEditBox_new00_local
static int tolua_gui_GuiEditBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiEditBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiEditBox* tolua_ret = (GuiEditBox*)  new GuiEditBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiEditBox");
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

/* method: new of class  GuiHeaderCtrl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiHeaderCtrl_new00
static int tolua_gui_GuiHeaderCtrl_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiHeaderCtrl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiHeaderCtrl* tolua_ret = (GuiHeaderCtrl*)  new GuiHeaderCtrl();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiHeaderCtrl");
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

/* method: new_local of class  GuiHeaderCtrl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiHeaderCtrl_new00_local
static int tolua_gui_GuiHeaderCtrl_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiHeaderCtrl",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiHeaderCtrl* tolua_ret = (GuiHeaderCtrl*)  new GuiHeaderCtrl();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiHeaderCtrl");
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

/* method: create of class  GuiHeaderCtrl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiHeaderCtrl_create00
static int tolua_gui_GuiHeaderCtrl_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiHeaderCtrl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiHeaderCtrl* self = (GuiHeaderCtrl*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  int style = ((int)  tolua_tonumber(tolua_S,3,0));
  GuiWnd* parent = ((GuiWnd*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(id,style,parent);
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

/* method: addColumn of class  GuiHeaderCtrl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiHeaderCtrl_addColumn00
static int tolua_gui_GuiHeaderCtrl_addColumn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiHeaderCtrl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiHeaderCtrl* self = (GuiHeaderCtrl*)  tolua_tousertype(tolua_S,1,0);
  int width = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addColumn'",NULL);
#endif
  {
   self->addColumn(width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addColumn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: remove of class  GuiHeaderCtrl */
#ifndef TOLUA_DISABLE_tolua_gui_GuiHeaderCtrl_remove00
static int tolua_gui_GuiHeaderCtrl_remove00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiHeaderCtrl",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiHeaderCtrl* self = (GuiHeaderCtrl*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'remove'",NULL);
#endif
  {
   self->remove(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_new00
static int tolua_gui_GuiListBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiListBox* tolua_ret = (GuiListBox*)  new GuiListBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiListBox");
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

/* method: new_local of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_new00_local
static int tolua_gui_GuiListBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiListBox* tolua_ret = (GuiListBox*)  new GuiListBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiListBox");
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

/* method: removeAll of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_removeAll00
static int tolua_gui_GuiListBox_removeAll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAll'",NULL);
#endif
  {
   self->removeAll();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAt of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_removeAt00
static int tolua_gui_GuiListBox_removeAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAt'",NULL);
#endif
  {
   self->removeAt(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addString of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_addString00
static int tolua_gui_GuiListBox_addString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addString'",NULL);
#endif
  {
   int tolua_ret = (int)  self->addString(str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: header of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_header00
static int tolua_gui_GuiListBox_header00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'header'",NULL);
#endif
  {
   GuiHeaderCtrl& tolua_ret = (GuiHeaderCtrl&)  self->header();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"GuiHeaderCtrl");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'header'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemText of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_getItemText00
static int tolua_gui_GuiListBox_getItemText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getItemText(index);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemData of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_getItemData00
static int tolua_gui_GuiListBox_getItemData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getItemData(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemCount of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_getItemCount00
static int tolua_gui_GuiListBox_getItemCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getItemCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurSel of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_getCurSel00
static int tolua_gui_GuiListBox_getCurSel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurSel'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCurSel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurSel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCurSel of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_setCurSel00
static int tolua_gui_GuiListBox_setCurSel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCurSel'",NULL);
#endif
  {
   self->setCurSel(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCurSel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setItemText of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_setItemText00
static int tolua_gui_GuiListBox_setItemText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* text = ((const char*)  tolua_tostring(tolua_S,3,0));
  int subitem = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setItemText'",NULL);
#endif
  {
   self->setItemText(index,text,subitem);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setItemText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setItemData of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_setItemData00
static int tolua_gui_GuiListBox_setItemData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int data = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setItemData'",NULL);
#endif
  {
   self->setItemData(index,data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setItemData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEditableColumn of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_setEditableColumn00
static int tolua_gui_GuiListBox_setEditableColumn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int column = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEditableColumn'",NULL);
#endif
  {
   self->setEditableColumn(column);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEditableColumn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: scrollTo of class  GuiListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiListBox_scrollTo00
static int tolua_gui_GuiListBox_scrollTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiListBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiListBox* self = (GuiListBox*)  tolua_tousertype(tolua_S,1,0);
  int idx = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'scrollTo'",NULL);
#endif
  {
   self->scrollTo(idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'scrollTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiFileListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiFileListBox_new00
static int tolua_gui_GuiFileListBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiFileListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiFileListBox* tolua_ret = (GuiFileListBox*)  new GuiFileListBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiFileListBox");
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

/* method: new_local of class  GuiFileListBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiFileListBox_new00_local
static int tolua_gui_GuiFileListBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiFileListBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiFileListBox* tolua_ret = (GuiFileListBox*)  new GuiFileListBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiFileListBox");
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

/* method: new of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_new00
static int tolua_gui_GuiComboBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiComboBox* tolua_ret = (GuiComboBox*)  new GuiComboBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiComboBox");
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

/* method: new_local of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_new00_local
static int tolua_gui_GuiComboBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiComboBox* tolua_ret = (GuiComboBox*)  new GuiComboBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiComboBox");
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

/* method: setCallback of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_setCallback00
static int tolua_gui_GuiComboBox_setCallback00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  const char* function = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCallback'",NULL);
#endif
  {
   self->setCallback(function);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCallback'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setCurSel of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_setCurSel00
static int tolua_gui_GuiComboBox_setCurSel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setCurSel'",NULL);
#endif
  {
   self->setCurSel(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setCurSel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setItemText of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_setItemText00
static int tolua_gui_GuiComboBox_setItemText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* text = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setItemText'",NULL);
#endif
  {
   self->setItemText(index,text);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setItemText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setItemData of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_setItemData00
static int tolua_gui_GuiComboBox_setItemData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int data = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setItemData'",NULL);
#endif
  {
   self->setItemData(index,data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setItemData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCurSel of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_getCurSel00
static int tolua_gui_GuiComboBox_getCurSel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCurSel'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getCurSel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCurSel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemText of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_getItemText00
static int tolua_gui_GuiComboBox_getItemText00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemText'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getItemText(index);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemText'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getItemData of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_getItemData00
static int tolua_gui_GuiComboBox_getItemData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getItemData'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getItemData(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getItemData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addString of class  GuiComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiComboBox_addString00
static int tolua_gui_GuiComboBox_addString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiComboBox",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiComboBox* self = (GuiComboBox*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addString'",NULL);
#endif
  {
   int tolua_ret = (int)  self->addString(str);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiColorComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColorComboBox_new00
static int tolua_gui_GuiColorComboBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiColorComboBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiColorComboBox* tolua_ret = (GuiColorComboBox*)  new GuiColorComboBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiColorComboBox");
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

/* method: new_local of class  GuiColorComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColorComboBox_new00_local
static int tolua_gui_GuiColorComboBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiColorComboBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiColorComboBox* tolua_ret = (GuiColorComboBox*)  new GuiColorComboBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiColorComboBox");
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

/* method: addColor of class  GuiColorComboBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiColorComboBox_addColor00
static int tolua_gui_GuiColorComboBox_addColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiColorComboBox",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiColorComboBox* self = (GuiColorComboBox*)  tolua_tousertype(tolua_S,1,0);
  GuiColor color = *((GuiColor*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addColor'",NULL);
#endif
  {
   self->addColor(color);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  GuiTreeBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiTreeBox_new00
static int tolua_gui_GuiTreeBox_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiTreeBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiTreeBox* tolua_ret = (GuiTreeBox*)  new GuiTreeBox();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiTreeBox");
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

/* method: new_local of class  GuiTreeBox */
#ifndef TOLUA_DISABLE_tolua_gui_GuiTreeBox_new00_local
static int tolua_gui_GuiTreeBox_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiTreeBox",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   GuiTreeBox* tolua_ret = (GuiTreeBox*)  new GuiTreeBox();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"GuiTreeBox");
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

/* method: addItem of class  GuiMenu */
#ifndef TOLUA_DISABLE_tolua_gui_GuiMenu_addItem00
static int tolua_gui_GuiMenu_addItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiMenu",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiMenu* self = (GuiMenu*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addItem'",NULL);
#endif
  {
   self->addItem(id,name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isShift of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_isShift00
static int tolua_gui_GuiCanvas_isShift00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   bool tolua_ret = (bool)  GuiCanvas::isShift();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isShift'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isAlt of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_isAlt00
static int tolua_gui_GuiCanvas_isAlt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   bool tolua_ret = (bool)  GuiCanvas::isAlt();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAlt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isCtrl of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_isCtrl00
static int tolua_gui_GuiCanvas_isCtrl00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   bool tolua_ret = (bool)  GuiCanvas::isCtrl();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isCtrl'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: quit of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_quit00
static int tolua_gui_GuiCanvas_quit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'quit'",NULL);
#endif
  {
   self->quit();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'quit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: doModal of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_doModal00
static int tolua_gui_GuiCanvas_doModal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
  GuiWnd* wnd = ((GuiWnd*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'doModal'",NULL);
#endif
  {
   self->doModal(wnd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'doModal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: endModal of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_endModal00
static int tolua_gui_GuiCanvas_endModal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
  int retValue = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'endModal'",NULL);
#endif
  {
   self->endModal(retValue);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'endModal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pushWindow of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_pushWindow00
static int tolua_gui_GuiCanvas_pushWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
  GuiWnd* wnd = ((GuiWnd*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pushWindow'",NULL);
#endif
  {
   self->pushWindow(wnd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pushWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: popWindow of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_popWindow00
static int tolua_gui_GuiCanvas_popWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
  GuiWnd* wnd = ((GuiWnd*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popWindow'",NULL);
#endif
  {
   self->popWindow(wnd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'popWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: popWindow of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_popWindow01
static int tolua_gui_GuiCanvas_popWindow01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'popWindow'",NULL);
#endif
  {
   self->popWindow();
  }
 }
 return 0;
tolua_lerror:
 return tolua_gui_GuiCanvas_popWindow00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeDefaultColor of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_changeDefaultColor00
static int tolua_gui_GuiCanvas_changeDefaultColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const GuiColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
  int color = ((int)  tolua_tonumber(tolua_S,2,0));
  const GuiColor* newColor = ((const GuiColor*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'changeDefaultColor'",NULL);
#endif
  {
   self->changeDefaultColor(color,*newColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'changeDefaultColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isActiveWnd of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_isActiveWnd00
static int tolua_gui_GuiCanvas_isActiveWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
  GuiWnd* window = ((GuiWnd*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isActiveWnd'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isActiveWnd(*window);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isActiveWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getActiveWnd of class  GuiCanvas */
#ifndef TOLUA_DISABLE_tolua_gui_GuiCanvas_getActiveWnd00
static int tolua_gui_GuiCanvas_getActiveWnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiCanvas",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiCanvas* self = (GuiCanvas*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getActiveWnd'",NULL);
#endif
  {
   GuiWnd& tolua_ret = (GuiWnd&)  self->getActiveWnd();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"GuiWnd");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getActiveWnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadFromXML of class  GuiManager */
#ifndef TOLUA_DISABLE_tolua_gui_GuiManager_loadFromXML00
static int tolua_gui_GuiManager_loadFromXML00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiManager* self = (GuiManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadFromXML'",NULL);
#endif
  {
   GuiDialog* tolua_ret = (GuiDialog*)  self->loadFromXML(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiDialog");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadFromXML'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadMenuFromXML of class  GuiManager */
#ifndef TOLUA_DISABLE_tolua_gui_GuiManager_loadMenuFromXML00
static int tolua_gui_GuiManager_loadMenuFromXML00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiManager* self = (GuiManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadMenuFromXML'",NULL);
#endif
  {
   GuiMenu* tolua_ret = (GuiMenu*)  self->loadMenuFromXML(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"GuiMenu");
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadMenuFromXML'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: showPopup of class  GuiManager */
#ifndef TOLUA_DISABLE_tolua_gui_GuiManager_showPopup00
static int tolua_gui_GuiManager_showPopup00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GuiManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuiWnd",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const GuiPoint",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GuiManager* self = (GuiManager*)  tolua_tousertype(tolua_S,1,0);
  GuiWnd* owner = ((GuiWnd*)  tolua_tousertype(tolua_S,2,0));
  const GuiPoint* point = ((const GuiPoint*)  tolua_tousertype(tolua_S,3,0));
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'showPopup'",NULL);
#endif
  {
   self->showPopup(*owner,*point,name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'showPopup'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_gui_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"SDL_EnableKeyRepeat",tolua_gui_SDL_EnableKeyRepeat00);
  tolua_constant(tolua_S,"GUI_TABSTOP",GUI_TABSTOP);
  tolua_constant(tolua_S,"GUI_ACTIVE",GUI_ACTIVE);
  tolua_constant(tolua_S,"GUI_PRESSED",GUI_PRESSED);
  tolua_constant(tolua_S,"GUI_VISIBLE",GUI_VISIBLE);
  tolua_constant(tolua_S,"GUI_NOTIFYPARENT",GUI_NOTIFYPARENT);
  tolua_constant(tolua_S,"GUI_NODESTROY",GUI_NODESTROY);
  tolua_constant(tolua_S,"GUI_TOPMOST",GUI_TOPMOST);
  tolua_constant(tolua_S,"GUI_BORDER",GUI_BORDER);
  tolua_constant(tolua_S,"GUI_BACKGROUND",GUI_BACKGROUND);
  tolua_constant(tolua_S,"CB_SETFOCUS",CB_SETFOCUS);
  tolua_constant(tolua_S,"CB_KILLFOCUS",CB_KILLFOCUS);
  tolua_cclass(tolua_S,"GuiFocus","GuiFocus","",NULL);
  tolua_beginmodule(tolua_S,"GuiFocus");
   tolua_function(tolua_S,"hasFocus",tolua_gui_GuiFocus_hasFocus00);
   tolua_function(tolua_S,"getFocus",tolua_gui_GuiFocus_getFocus00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiPoint","GuiPoint","",tolua_collect_GuiPoint);
  #else
  tolua_cclass(tolua_S,"GuiPoint","GuiPoint","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiPoint");
   tolua_function(tolua_S,"new",tolua_gui_GuiPoint_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiPoint_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiPoint_new00_local);
   tolua_function(tolua_S,"new",tolua_gui_GuiPoint_new01);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiPoint_new01_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiPoint_new01_local);
   tolua_function(tolua_S,"new",tolua_gui_GuiPoint_new02);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiPoint_new02_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiPoint_new02_local);
   tolua_variable(tolua_S,"x",tolua_get_GuiPoint_x,tolua_set_GuiPoint_x);
   tolua_variable(tolua_S,"y",tolua_get_GuiPoint_y,tolua_set_GuiPoint_y);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiColor","GuiColor","",tolua_collect_GuiColor);
  #else
  tolua_cclass(tolua_S,"GuiColor","GuiColor","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiColor");
   tolua_function(tolua_S,"new",tolua_gui_GuiColor_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiColor_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiColor_new00_local);
   tolua_function(tolua_S,"new",tolua_gui_GuiColor_new01);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiColor_new01_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiColor_new01_local);
   tolua_variable(tolua_S,"r",tolua_get_GuiColor_r,tolua_set_GuiColor_r);
   tolua_variable(tolua_S,"g",tolua_get_GuiColor_g,tolua_set_GuiColor_g);
   tolua_variable(tolua_S,"b",tolua_get_GuiColor_b,tolua_set_GuiColor_b);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiRect","GuiRect","",tolua_collect_GuiRect);
  #else
  tolua_cclass(tolua_S,"GuiRect","GuiRect","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiRect");
   tolua_function(tolua_S,"new",tolua_gui_GuiRect_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiRect_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiRect_new00_local);
   tolua_function(tolua_S,"new",tolua_gui_GuiRect_new01);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiRect_new01_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiRect_new01_local);
   tolua_function(tolua_S,"set",tolua_gui_GuiRect_set00);
   tolua_function(tolua_S,"pointInRect",tolua_gui_GuiRect_pointInRect00);
   tolua_function(tolua_S,"left",tolua_gui_GuiRect_left00);
   tolua_function(tolua_S,"right",tolua_gui_GuiRect_right00);
   tolua_function(tolua_S,"top",tolua_gui_GuiRect_top00);
   tolua_function(tolua_S,"bottom",tolua_gui_GuiRect_bottom00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GuiEventHandlerDefinition","GuiEventHandlerDefinition","",NULL);
  tolua_beginmodule(tolua_S,"GuiEventHandlerDefinition");
   tolua_function(tolua_S,"getName",tolua_gui_GuiEventHandlerDefinition_getName00);
   tolua_function(tolua_S,"getCode",tolua_gui_GuiEventHandlerDefinition_getCode00);
   tolua_function(tolua_S,"setCode",tolua_gui_GuiEventHandlerDefinition_setCode00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GuiEventHandlerDefinitions","GuiEventHandlerDefinitions","",NULL);
  tolua_beginmodule(tolua_S,"GuiEventHandlerDefinitions");
   tolua_function(tolua_S,"getDefinition",tolua_gui_GuiEventHandlerDefinitions_getDefinition00);
   tolua_function(tolua_S,"size",tolua_gui_GuiEventHandlerDefinitions_size00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GuiWnd","GuiWnd","",NULL);
  tolua_beginmodule(tolua_S,"GuiWnd");
   tolua_function(tolua_S,"create",tolua_gui_GuiWnd_create00);
   tolua_function(tolua_S,"addChild",tolua_gui_GuiWnd_addChild00);
   tolua_function(tolua_S,"getId",tolua_gui_GuiWnd_getId00);
   tolua_function(tolua_S,"getWindowRect",tolua_gui_GuiWnd_getWindowRect00);
   tolua_function(tolua_S,"moveWindow",tolua_gui_GuiWnd_moveWindow00);
   tolua_function(tolua_S,"setWindowPos",tolua_gui_GuiWnd_setWindowPos00);
   tolua_function(tolua_S,"setVisible",tolua_gui_GuiWnd_setVisible00);
   tolua_function(tolua_S,"setCaption",tolua_gui_GuiWnd_setCaption00);
   tolua_function(tolua_S,"setBackgroundImage",tolua_gui_GuiWnd_setBackgroundImage00);
   tolua_function(tolua_S,"setFocus",tolua_gui_GuiWnd_setFocus00);
   tolua_function(tolua_S,"center",tolua_gui_GuiWnd_center00);
   tolua_function(tolua_S,"clientToWindow",tolua_gui_GuiWnd_clientToWindow00);
   tolua_function(tolua_S,"windowToClient",tolua_gui_GuiWnd_windowToClient00);
   tolua_function(tolua_S,"windowToClient",tolua_gui_GuiWnd_windowToClient01);
   tolua_function(tolua_S,"getParent",tolua_gui_GuiWnd_getParent00);
   tolua_function(tolua_S,"getItemById",tolua_gui_GuiWnd_getItemById00);
   tolua_function(tolua_S,"findChildByName",tolua_gui_GuiWnd_findChildByName00);
   tolua_function(tolua_S,"getCaption",tolua_gui_GuiWnd_getCaption00);
   tolua_function(tolua_S,"getEventHandlerDefinitions",tolua_gui_GuiWnd_getEventHandlerDefinitions00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiWindow","GuiWindow","GuiWnd",tolua_collect_GuiWindow);
  #else
  tolua_cclass(tolua_S,"GuiWindow","GuiWindow","GuiWnd",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiWindow");
   tolua_function(tolua_S,"new",tolua_gui_GuiWindow_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiWindow_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiWindow_new00_local);
   tolua_function(tolua_S,"setActive",tolua_gui_GuiWindow_setActive00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiScriptWnd","GuiScriptWnd","GuiWindow",tolua_collect_GuiScriptWnd);
  #else
  tolua_cclass(tolua_S,"GuiScriptWnd","GuiScriptWnd","GuiWindow",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiScriptWnd");
   tolua_function(tolua_S,"new",tolua_gui_GuiScriptWnd_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiScriptWnd_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiScriptWnd_new00_local);
   tolua_function(tolua_S,"delete",tolua_gui_GuiScriptWnd_delete00);
   tolua_function(tolua_S,"setMouseMoveFnc",tolua_gui_GuiScriptWnd_setMouseMoveFnc00);
   tolua_function(tolua_S,"setLButtonDownFnc",tolua_gui_GuiScriptWnd_setLButtonDownFnc00);
   tolua_function(tolua_S,"setKeyDownFnc",tolua_gui_GuiScriptWnd_setKeyDownFnc00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiGameWnd","GuiGameWnd","GuiScriptWnd",tolua_collect_GuiGameWnd);
  #else
  tolua_cclass(tolua_S,"GuiGameWnd","GuiGameWnd","GuiScriptWnd",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiGameWnd");
   tolua_function(tolua_S,"new",tolua_gui_GuiGameWnd_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiGameWnd_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiGameWnd_new00_local);
   tolua_function(tolua_S,"pushMap",tolua_gui_GuiGameWnd_pushMap00);
   tolua_function(tolua_S,"popMap",tolua_gui_GuiGameWnd_popMap00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiControl","GuiControl","GuiWnd",tolua_collect_GuiControl);
  #else
  tolua_cclass(tolua_S,"GuiControl","GuiControl","GuiWnd",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiControl");
   tolua_function(tolua_S,"new",tolua_gui_GuiControl_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiControl_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiControl_new00_local);
   tolua_function(tolua_S,"delete",tolua_gui_GuiControl_delete00);
   tolua_function(tolua_S,"setCallback",tolua_gui_GuiControl_setCallback00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiDialog","GuiDialog","GuiWnd",tolua_collect_GuiDialog);
  #else
  tolua_cclass(tolua_S,"GuiDialog","GuiDialog","GuiWnd",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiDialog");
   tolua_function(tolua_S,"new",tolua_gui_GuiDialog_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiDialog_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiDialog_new00_local);
   tolua_function(tolua_S,"doModal",tolua_gui_GuiDialog_doModal00);
   tolua_function(tolua_S,"endModal",tolua_gui_GuiDialog_endModal00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiStatic","GuiStatic","GuiControl",tolua_collect_GuiStatic);
  #else
  tolua_cclass(tolua_S,"GuiStatic","GuiStatic","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiStatic");
   tolua_function(tolua_S,"new",tolua_gui_GuiStatic_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiStatic_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiStatic_new00_local);
   tolua_function(tolua_S,"new",tolua_gui_GuiStatic_new01);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiStatic_new01_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiStatic_new01_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiButton","GuiButton","GuiControl",tolua_collect_GuiButton);
  #else
  tolua_cclass(tolua_S,"GuiButton","GuiButton","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiButton");
   tolua_function(tolua_S,"new",tolua_gui_GuiButton_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiButton_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiButton_new00_local);
   tolua_function(tolua_S,"setCommand",tolua_gui_GuiButton_setCommand00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiImageButton","GuiImageButton","GuiButton",tolua_collect_GuiImageButton);
  #else
  tolua_cclass(tolua_S,"GuiImageButton","GuiImageButton","GuiButton",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiImageButton");
   tolua_function(tolua_S,"new",tolua_gui_GuiImageButton_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiImageButton_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiImageButton_new00_local);
   tolua_function(tolua_S,"setImage",tolua_gui_GuiImageButton_setImage00);
   tolua_function(tolua_S,"setPressedImage",tolua_gui_GuiImageButton_setPressedImage00);
   tolua_function(tolua_S,"setTextureCoordinates",tolua_gui_GuiImageButton_setTextureCoordinates00);
   tolua_function(tolua_S,"setOffset",tolua_gui_GuiImageButton_setOffset00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiCheckBox","GuiCheckBox","GuiControl",tolua_collect_GuiCheckBox);
  #else
  tolua_cclass(tolua_S,"GuiCheckBox","GuiCheckBox","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiCheckBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiCheckBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiCheckBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiCheckBox_new00_local);
   tolua_function(tolua_S,"isChecked",tolua_gui_GuiCheckBox_isChecked00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiImage","GuiImage","GuiWnd",tolua_collect_GuiImage);
  #else
  tolua_cclass(tolua_S,"GuiImage","GuiImage","GuiWnd",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiImage");
   tolua_function(tolua_S,"new",tolua_gui_GuiImage_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiImage_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiImage_new00_local);
   tolua_function(tolua_S,"setImage",tolua_gui_GuiImage_setImage00);
   tolua_function(tolua_S,"setOffset",tolua_gui_GuiImage_setOffset00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiMovie","GuiMovie","GuiImage",tolua_collect_GuiMovie);
  #else
  tolua_cclass(tolua_S,"GuiMovie","GuiMovie","GuiImage",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiMovie");
   tolua_function(tolua_S,"new",tolua_gui_GuiMovie_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiMovie_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiMovie_new00_local);
   tolua_function(tolua_S,"setMovie",tolua_gui_GuiMovie_setMovie00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiEditBox","GuiEditBox","GuiControl",tolua_collect_GuiEditBox);
  #else
  tolua_cclass(tolua_S,"GuiEditBox","GuiEditBox","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiEditBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiEditBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiEditBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiEditBox_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiHeaderCtrl","GuiHeaderCtrl","GuiControl",tolua_collect_GuiHeaderCtrl);
  #else
  tolua_cclass(tolua_S,"GuiHeaderCtrl","GuiHeaderCtrl","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiHeaderCtrl");
   tolua_function(tolua_S,"new",tolua_gui_GuiHeaderCtrl_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiHeaderCtrl_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiHeaderCtrl_new00_local);
   tolua_function(tolua_S,"create",tolua_gui_GuiHeaderCtrl_create00);
   tolua_function(tolua_S,"addColumn",tolua_gui_GuiHeaderCtrl_addColumn00);
   tolua_function(tolua_S,"remove",tolua_gui_GuiHeaderCtrl_remove00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiListBox","GuiListBox","GuiControl",tolua_collect_GuiListBox);
  #else
  tolua_cclass(tolua_S,"GuiListBox","GuiListBox","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiListBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiListBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiListBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiListBox_new00_local);
   tolua_function(tolua_S,"removeAll",tolua_gui_GuiListBox_removeAll00);
   tolua_function(tolua_S,"removeAt",tolua_gui_GuiListBox_removeAt00);
   tolua_function(tolua_S,"addString",tolua_gui_GuiListBox_addString00);
   tolua_function(tolua_S,"header",tolua_gui_GuiListBox_header00);
   tolua_function(tolua_S,"getItemText",tolua_gui_GuiListBox_getItemText00);
   tolua_function(tolua_S,"getItemData",tolua_gui_GuiListBox_getItemData00);
   tolua_function(tolua_S,"getItemCount",tolua_gui_GuiListBox_getItemCount00);
   tolua_function(tolua_S,"getCurSel",tolua_gui_GuiListBox_getCurSel00);
   tolua_function(tolua_S,"setCurSel",tolua_gui_GuiListBox_setCurSel00);
   tolua_function(tolua_S,"setItemText",tolua_gui_GuiListBox_setItemText00);
   tolua_function(tolua_S,"setItemData",tolua_gui_GuiListBox_setItemData00);
   tolua_function(tolua_S,"setEditableColumn",tolua_gui_GuiListBox_setEditableColumn00);
   tolua_function(tolua_S,"scrollTo",tolua_gui_GuiListBox_scrollTo00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiFileListBox","GuiFileListBox","GuiListBox",tolua_collect_GuiFileListBox);
  #else
  tolua_cclass(tolua_S,"GuiFileListBox","GuiFileListBox","GuiListBox",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiFileListBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiFileListBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiFileListBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiFileListBox_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiComboBox","GuiComboBox","GuiControl",tolua_collect_GuiComboBox);
  #else
  tolua_cclass(tolua_S,"GuiComboBox","GuiComboBox","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiComboBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiComboBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiComboBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiComboBox_new00_local);
   tolua_function(tolua_S,"setCallback",tolua_gui_GuiComboBox_setCallback00);
   tolua_function(tolua_S,"setCurSel",tolua_gui_GuiComboBox_setCurSel00);
   tolua_function(tolua_S,"setItemText",tolua_gui_GuiComboBox_setItemText00);
   tolua_function(tolua_S,"setItemData",tolua_gui_GuiComboBox_setItemData00);
   tolua_function(tolua_S,"getCurSel",tolua_gui_GuiComboBox_getCurSel00);
   tolua_function(tolua_S,"getItemText",tolua_gui_GuiComboBox_getItemText00);
   tolua_function(tolua_S,"getItemData",tolua_gui_GuiComboBox_getItemData00);
   tolua_function(tolua_S,"addString",tolua_gui_GuiComboBox_addString00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiColorComboBox","GuiColorComboBox","GuiComboBox",tolua_collect_GuiColorComboBox);
  #else
  tolua_cclass(tolua_S,"GuiColorComboBox","GuiColorComboBox","GuiComboBox",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiColorComboBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiColorComboBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiColorComboBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiColorComboBox_new00_local);
   tolua_function(tolua_S,"addColor",tolua_gui_GuiColorComboBox_addColor00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"GuiTreeBox","GuiTreeBox","GuiControl",tolua_collect_GuiTreeBox);
  #else
  tolua_cclass(tolua_S,"GuiTreeBox","GuiTreeBox","GuiControl",NULL);
  #endif
  tolua_beginmodule(tolua_S,"GuiTreeBox");
   tolua_function(tolua_S,"new",tolua_gui_GuiTreeBox_new00);
   tolua_function(tolua_S,"new_local",tolua_gui_GuiTreeBox_new00_local);
   tolua_function(tolua_S,".call",tolua_gui_GuiTreeBox_new00_local);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GuiMenu","GuiMenu","GuiControl",NULL);
  tolua_beginmodule(tolua_S,"GuiMenu");
   tolua_function(tolua_S,"addItem",tolua_gui_GuiMenu_addItem00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GuiCanvas","GuiCanvas","GuiWnd",NULL);
  tolua_beginmodule(tolua_S,"GuiCanvas");
   tolua_constant(tolua_S,"GuiWindowColor",GuiCanvas::GuiWindowColor);
   tolua_constant(tolua_S,"GuiBorderColor",GuiCanvas::GuiBorderColor);
   tolua_constant(tolua_S,"GuiHeaderColor",GuiCanvas::GuiHeaderColor);
   tolua_constant(tolua_S,"MaxGuiColors",GuiCanvas::MaxGuiColors);
   tolua_function(tolua_S,"isShift",tolua_gui_GuiCanvas_isShift00);
   tolua_function(tolua_S,"isAlt",tolua_gui_GuiCanvas_isAlt00);
   tolua_function(tolua_S,"isCtrl",tolua_gui_GuiCanvas_isCtrl00);
   tolua_function(tolua_S,"quit",tolua_gui_GuiCanvas_quit00);
   tolua_function(tolua_S,"doModal",tolua_gui_GuiCanvas_doModal00);
   tolua_function(tolua_S,"endModal",tolua_gui_GuiCanvas_endModal00);
   tolua_function(tolua_S,"pushWindow",tolua_gui_GuiCanvas_pushWindow00);
   tolua_function(tolua_S,"popWindow",tolua_gui_GuiCanvas_popWindow00);
   tolua_function(tolua_S,"popWindow",tolua_gui_GuiCanvas_popWindow01);
   tolua_function(tolua_S,"changeDefaultColor",tolua_gui_GuiCanvas_changeDefaultColor00);
   tolua_function(tolua_S,"isActiveWnd",tolua_gui_GuiCanvas_isActiveWnd00);
   tolua_function(tolua_S,"getActiveWnd",tolua_gui_GuiCanvas_getActiveWnd00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GuiManager","GuiManager","",NULL);
  tolua_beginmodule(tolua_S,"GuiManager");
   tolua_function(tolua_S,"loadFromXML",tolua_gui_GuiManager_loadFromXML00);
   tolua_function(tolua_S,"loadMenuFromXML",tolua_gui_GuiManager_loadMenuFromXML00);
   tolua_function(tolua_S,"showPopup",tolua_gui_GuiManager_showPopup00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_gui (lua_State* tolua_S) {
 return tolua_gui_open(tolua_S);
};
#endif

