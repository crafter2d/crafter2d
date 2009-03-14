/*
** Lua binding: general
** Generated automatically by tolua++-1.0.92 on 03/13/09 16:16:12.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_general_open (lua_State* tolua_S);

#include "math/vector.h"
#include "world/world.h"
#include "world/layer.h"
#include "world/isodiamondlayer.h"
#include "world/topdownlayer.h"
#include "world/bound.h"
#include "actionmap.h"
#include "creature.h"
#include "particlesystem.h"
#include "weapon.h"
#include "console.h"
#include "state.h"
#include "script.h"
#include "scriptmanager.h"
#include "sound.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_RotateState (lua_State* tolua_S)
{
 RotateState* self = (RotateState*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_KeyMap (lua_State* tolua_S)
{
 KeyMap* self = (KeyMap*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Creature (lua_State* tolua_S)
{
 Creature* self = (Creature*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_MoveState (lua_State* tolua_S)
{
 MoveState* self = (MoveState*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Color (lua_State* tolua_S)
{
 Color* self = (Color*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_ActionMap (lua_State* tolua_S)
{
 ActionMap* self = (ActionMap*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_TopDownLayer (lua_State* tolua_S)
{
 TopDownLayer* self = (TopDownLayer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_World (lua_State* tolua_S)
{
 World* self = (World*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Bound (lua_State* tolua_S)
{
 Bound* self = (Bound*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_IsoDiamondLayer (lua_State* tolua_S)
{
 IsoDiamondLayer* self = (IsoDiamondLayer*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Object (lua_State* tolua_S)
{
 Object* self = (Object*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Vector (lua_State* tolua_S)
{
 Vector* self = (Vector*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_ParticleSystem (lua_State* tolua_S)
{
 ParticleSystem* self = (ParticleSystem*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_AnimObject (lua_State* tolua_S)
{
 AnimObject* self = (AnimObject*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_LocationState (lua_State* tolua_S)
{
 LocationState* self = (LocationState*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_Weapon (lua_State* tolua_S)
{
 Weapon* self = (Weapon*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"RotateState");
 tolua_usertype(tolua_S,"Console");
 tolua_usertype(tolua_S,"MoveState");
 tolua_usertype(tolua_S,"Color");
 tolua_usertype(tolua_S,"TopDownLayer");
 tolua_usertype(tolua_S,"Vector");
 tolua_usertype(tolua_S,"ParticleSystem");
 tolua_usertype(tolua_S,"Sound");
 tolua_usertype(tolua_S,"World");
 tolua_usertype(tolua_S,"LocationState");
 tolua_usertype(tolua_S,"ScriptManager");
 tolua_usertype(tolua_S,"Creature");
 tolua_usertype(tolua_S,"SoundManager");
 tolua_usertype(tolua_S,"KeyMap");
 tolua_usertype(tolua_S,"Layer");
 tolua_usertype(tolua_S,"Particle");
 tolua_usertype(tolua_S,"SceneObject");
 tolua_usertype(tolua_S,"State");
 tolua_usertype(tolua_S,"ActionMap");
 tolua_usertype(tolua_S,"Object");
 tolua_usertype(tolua_S,"SceneGraph");
 tolua_usertype(tolua_S,"Bound");
 tolua_usertype(tolua_S,"AnimObject");
 tolua_usertype(tolua_S,"IsoDiamondLayer");
 tolua_usertype(tolua_S,"Weapon");
}

/* method: new of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_new00
static int tolua_general_Vector_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Vector* tolua_ret = (Vector*)  new Vector();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector");
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

/* method: new_local of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_new00_local
static int tolua_general_Vector_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Vector* tolua_ret = (Vector*)  new Vector();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Vector");
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

/* method: new of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_new01
static int tolua_general_Vector_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   Vector* tolua_ret = (Vector*)  new Vector(x,y);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Vector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_Vector_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_new01_local
static int tolua_general_Vector_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   Vector* tolua_ret = (Vector*)  new Vector(x,y);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Vector");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_Vector_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: normalize of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_normalize00
static int tolua_general_Vector_normalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'normalize'",NULL);
#endif
  {
   self->normalize();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'normalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: length of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_length00
static int tolua_general_Vector_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length'",NULL);
#endif
  {
   float tolua_ret = (float)  self->length();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dot of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_dot00
static int tolua_general_Vector_dot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* v = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dot'",NULL);
#endif
  {
   float tolua_ret = (float)  self->dot(*v);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector_set00
static int tolua_general_Vector_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  float x = ((float)  tolua_tonumber(tolua_S,2,0));
  float y = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(x,y);
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

/* method: operator+ of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector__add00
static int tolua_general_Vector__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* v = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->operator+(*v);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Vector */
#ifndef TOLUA_DISABLE_tolua_general_Vector__sub00
static int tolua_general_Vector__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
  const Vector* v = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->operator-(*v);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Vector */
#ifndef TOLUA_DISABLE_tolua_get_Vector_x
static int tolua_get_Vector_x(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Vector */
#ifndef TOLUA_DISABLE_tolua_set_Vector_x
static int tolua_set_Vector_x(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Vector */
#ifndef TOLUA_DISABLE_tolua_get_Vector_y
static int tolua_get_Vector_y(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Vector */
#ifndef TOLUA_DISABLE_tolua_set_Vector_y
static int tolua_set_Vector_y(lua_State* tolua_S)
{
  Vector* self = (Vector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Color */
#ifndef TOLUA_DISABLE_tolua_general_Color_new00
static int tolua_general_Color_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Color* tolua_ret = (Color*)  new Color();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Color");
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

/* method: new_local of class  Color */
#ifndef TOLUA_DISABLE_tolua_general_Color_new00_local
static int tolua_general_Color_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Color",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Color* tolua_ret = (Color*)  new Color();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Color");
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

/* method: new of class  Color */
#ifndef TOLUA_DISABLE_tolua_general_Color_new01
static int tolua_general_Color_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  float a = ((float)  tolua_tonumber(tolua_S,5,1.0f));
  {
   Color* tolua_ret = (Color*)  new Color(r,g,b,a);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_Color_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Color */
#ifndef TOLUA_DISABLE_tolua_general_Color_new01_local
static int tolua_general_Color_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
  float a = ((float)  tolua_tonumber(tolua_S,5,1.0f));
  {
   Color* tolua_ret = (Color*)  new Color(r,g,b,a);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Color");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_Color_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  Color */
#ifndef TOLUA_DISABLE_tolua_general_Color_set00
static int tolua_general_Color_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Color",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(r,g,b);
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

/* get function: r of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_r
static int tolua_get_Color_r(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->r);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: r of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_r
static int tolua_set_Color_r(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: g of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_g
static int tolua_get_Color_g(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->g);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_g
static int tolua_set_Color_g(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: b of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_b
static int tolua_get_Color_b(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->b);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_b
static int tolua_set_Color_b(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: a of class  Color */
#ifndef TOLUA_DISABLE_tolua_get_Color_a
static int tolua_get_Color_a(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->a);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: a of class  Color */
#ifndef TOLUA_DISABLE_tolua_set_Color_a
static int tolua_set_Color_a(lua_State* tolua_S)
{
  Color* self = (Color*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->a = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: setColor of class  Particle */
#ifndef TOLUA_DISABLE_tolua_general_Particle_setColor00
static int tolua_general_Particle_setColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Particle",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
  float r = ((float)  tolua_tonumber(tolua_S,2,0));
  float g = ((float)  tolua_tonumber(tolua_S,3,0));
  float b = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setColor'",NULL);
#endif
  {
   self->setColor(r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: color of class  Particle */
#ifndef TOLUA_DISABLE_tolua_get_Particle_color
static int tolua_get_Particle_color(lua_State* tolua_S)
{
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->color,"Color");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: color of class  Particle */
#ifndef TOLUA_DISABLE_tolua_set_Particle_color
static int tolua_set_Particle_color(lua_State* tolua_S)
{
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
  if (!tolua_isusertype(tolua_S,2,"Color",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->color = *((Color*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: life of class  Particle */
#ifndef TOLUA_DISABLE_tolua_get_Particle_life
static int tolua_get_Particle_life(lua_State* tolua_S)
{
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'life'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->life);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: life of class  Particle */
#ifndef TOLUA_DISABLE_tolua_set_Particle_life
static int tolua_set_Particle_life(lua_State* tolua_S)
{
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'life'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->life = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: state of class  Particle */
#ifndef TOLUA_DISABLE_tolua_get_Particle_state
static int tolua_get_Particle_state(lua_State* tolua_S)
{
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'state'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->state);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: state of class  Particle */
#ifndef TOLUA_DISABLE_tolua_set_Particle_state
static int tolua_set_Particle_state(lua_State* tolua_S)
{
  Particle* self = (Particle*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'state'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->state = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: duration of class  State */
#ifndef TOLUA_DISABLE_tolua_general_State_duration00
static int tolua_general_State_duration00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"State",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  State* self = (State*)  tolua_tousertype(tolua_S,1,0);
  int time = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'duration'",NULL);
#endif
  {
   self->duration(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'duration'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: duration of class  State */
#ifndef TOLUA_DISABLE_tolua_general_State_duration01
static int tolua_general_State_duration01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const State",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const State* self = (const State*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'duration'",NULL);
#endif
  {
   int tolua_ret = (int)  self->duration();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_State_duration00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: start of class  State */
#ifndef TOLUA_DISABLE_tolua_general_State_start00
static int tolua_general_State_start00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"State",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  State* self = (State*)  tolua_tousertype(tolua_S,1,0);
  int s = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'",NULL);
#endif
  {
   self->start(s);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'start'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: start of class  State */
#ifndef TOLUA_DISABLE_tolua_general_State_start01
static int tolua_general_State_start01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const State",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const State* self = (const State*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'start'",NULL);
#endif
  {
   int tolua_ret = (int)  self->start();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_State_start00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  LocationState */
#ifndef TOLUA_DISABLE_tolua_general_LocationState_new00
static int tolua_general_LocationState_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LocationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LocationState* tolua_ret = (LocationState*)  new LocationState();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"LocationState");
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

/* method: new_local of class  LocationState */
#ifndef TOLUA_DISABLE_tolua_general_LocationState_new00_local
static int tolua_general_LocationState_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LocationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LocationState* tolua_ret = (LocationState*)  new LocationState();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"LocationState");
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

/* method: new of class  LocationState */
#ifndef TOLUA_DISABLE_tolua_general_LocationState_new01
static int tolua_general_LocationState_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LocationState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector* location = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
  {
   LocationState* tolua_ret = (LocationState*)  new LocationState(*location);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"LocationState");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_LocationState_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  LocationState */
#ifndef TOLUA_DISABLE_tolua_general_LocationState_new01_local
static int tolua_general_LocationState_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LocationState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Vector* location = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
  {
   LocationState* tolua_ret = (LocationState*)  new LocationState(*location);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"LocationState");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_LocationState_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: location of class  LocationState */
#ifndef TOLUA_DISABLE_tolua_general_LocationState_location00
static int tolua_general_LocationState_location00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LocationState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LocationState* self = (LocationState*)  tolua_tousertype(tolua_S,1,0);
  const Vector* loc = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'location'",NULL);
#endif
  {
   self->location(*loc);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'location'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: location of class  LocationState */
#ifndef TOLUA_DISABLE_tolua_general_LocationState_location01
static int tolua_general_LocationState_location01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const LocationState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const LocationState* self = (const LocationState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'location'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->location();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_LocationState_location00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MoveState */
#ifndef TOLUA_DISABLE_tolua_general_MoveState_new00
static int tolua_general_MoveState_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MoveState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MoveState* tolua_ret = (MoveState*)  new MoveState();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"MoveState");
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

/* method: new_local of class  MoveState */
#ifndef TOLUA_DISABLE_tolua_general_MoveState_new00_local
static int tolua_general_MoveState_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MoveState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MoveState* tolua_ret = (MoveState*)  new MoveState();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"MoveState");
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

/* method: new of class  MoveState */
#ifndef TOLUA_DISABLE_tolua_general_MoveState_new01
static int tolua_general_MoveState_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MoveState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int duration = ((int)  tolua_tonumber(tolua_S,2,0));
  const Vector* destination = ((const Vector*)  tolua_tousertype(tolua_S,3,0));
  {
   MoveState* tolua_ret = (MoveState*)  new MoveState(duration,*destination);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"MoveState");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_MoveState_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  MoveState */
#ifndef TOLUA_DISABLE_tolua_general_MoveState_new01_local
static int tolua_general_MoveState_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MoveState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int duration = ((int)  tolua_tonumber(tolua_S,2,0));
  const Vector* destination = ((const Vector*)  tolua_tousertype(tolua_S,3,0));
  {
   MoveState* tolua_ret = (MoveState*)  new MoveState(duration,*destination);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"MoveState");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_MoveState_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: destination of class  MoveState */
#ifndef TOLUA_DISABLE_tolua_general_MoveState_destination00
static int tolua_general_MoveState_destination00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MoveState",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MoveState* self = (MoveState*)  tolua_tousertype(tolua_S,1,0);
  const Vector* dest = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destination'",NULL);
#endif
  {
   self->destination(*dest);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destination'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destination of class  MoveState */
#ifndef TOLUA_DISABLE_tolua_general_MoveState_destination01
static int tolua_general_MoveState_destination01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const MoveState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const MoveState* self = (const MoveState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'destination'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->destination();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_MoveState_destination00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  RotateState */
#ifndef TOLUA_DISABLE_tolua_general_RotateState_new00
static int tolua_general_RotateState_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RotateState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RotateState* tolua_ret = (RotateState*)  new RotateState();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"RotateState");
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

/* method: new_local of class  RotateState */
#ifndef TOLUA_DISABLE_tolua_general_RotateState_new00_local
static int tolua_general_RotateState_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RotateState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RotateState* tolua_ret = (RotateState*)  new RotateState();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RotateState");
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

/* method: new of class  RotateState */
#ifndef TOLUA_DISABLE_tolua_general_RotateState_new01
static int tolua_general_RotateState_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RotateState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int duration = ((int)  tolua_tonumber(tolua_S,2,0));
  float rotation = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   RotateState* tolua_ret = (RotateState*)  new RotateState(duration,rotation);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"RotateState");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_RotateState_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  RotateState */
#ifndef TOLUA_DISABLE_tolua_general_RotateState_new01_local
static int tolua_general_RotateState_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RotateState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int duration = ((int)  tolua_tonumber(tolua_S,2,0));
  float rotation = ((float)  tolua_tonumber(tolua_S,3,0));
  {
   RotateState* tolua_ret = (RotateState*)  new RotateState(duration,rotation);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"RotateState");
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_RotateState_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotation of class  RotateState */
#ifndef TOLUA_DISABLE_tolua_general_RotateState_rotation00
static int tolua_general_RotateState_rotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RotateState",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RotateState* self = (RotateState*)  tolua_tousertype(tolua_S,1,0);
  float angle = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotation'",NULL);
#endif
  {
   self->rotation(angle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotation of class  RotateState */
#ifndef TOLUA_DISABLE_tolua_general_RotateState_rotation01
static int tolua_general_RotateState_rotation01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const RotateState",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const RotateState* self = (const RotateState*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotation'",NULL);
#endif
  {
   float tolua_ret = (float)  self->rotation();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_general_RotateState_rotation00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: add of class  SceneObject */
#ifndef TOLUA_DISABLE_tolua_general_SceneObject_add00
static int tolua_general_SceneObject_add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneObject",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"SceneObject",0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneObject* self = (SceneObject*)  tolua_tousertype(tolua_S,1,0);
  SceneObject* child = ((SceneObject*)  tolua_tousertype(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,NULL));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add'",NULL);
#endif
  {
   self->add(child,name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: find of class  SceneObject */
#ifndef TOLUA_DISABLE_tolua_general_SceneObject_find00
static int tolua_general_SceneObject_find00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneObject",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneObject* self = (SceneObject*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'find'",NULL);
#endif
  {
   SceneObject* tolua_ret = (SceneObject*)  self->find(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"SceneObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setName of class  SceneObject */
#ifndef TOLUA_DISABLE_tolua_general_SceneObject_setName00
static int tolua_general_SceneObject_setName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneObject",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneObject* self = (SceneObject*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setName'",NULL);
#endif
  {
   self->setName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  SceneObject */
#ifndef TOLUA_DISABLE_tolua_general_SceneObject_getName00
static int tolua_general_SceneObject_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneObject* self = (SceneObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getName'",NULL);
#endif
  {
   char* tolua_ret = (char*)  self->getName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
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

/* method: new of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_new00
static int tolua_general_Object_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Object* tolua_ret = (Object*)  new Object();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
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

/* method: new_local of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_new00_local
static int tolua_general_Object_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Object* tolua_ret = (Object*)  new Object();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Object");
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

/* method: create of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_create00
static int tolua_general_Object_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  const char* filename = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(filename);
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

/* method: flip of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_flip00
static int tolua_general_Object_flip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flip'",NULL);
#endif
  {
   self->flip();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: direction of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_direction00
static int tolua_general_Object_direction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'direction'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->direction();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'direction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotate of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_rotate00
static int tolua_general_Object_rotate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  float deg = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotate'",NULL);
#endif
  {
   self->rotate(deg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPosition of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_setPosition00
static int tolua_general_Object_setPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  const Vector* pos = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPosition'",NULL);
#endif
  {
   self->setPosition(*pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setVelocity of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_setVelocity00
static int tolua_general_Object_setVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  const Vector* vel = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setVelocity'",NULL);
#endif
  {
   self->setVelocity(*vel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotation of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_setRotation00
static int tolua_general_Object_setRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  const float deg = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotation'",NULL);
#endif
  {
   self->setRotation(deg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMoveSpeed of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_setMoveSpeed00
static int tolua_general_Object_setMoveSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  int speed = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMoveSpeed'",NULL);
#endif
  {
   self->setMoveSpeed(speed);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMoveSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getPosition of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_getPosition00
static int tolua_general_Object_getPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPosition'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->getPosition();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVelocity of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_getVelocity00
static int tolua_general_Object_getVelocity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVelocity'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->getVelocity();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVelocity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotation of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_getRotation00
static int tolua_general_Object_getRotation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotation'",NULL);
#endif
  {
   float tolua_ret = (float)  self->getRotation();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addState of class  Object */
#ifndef TOLUA_DISABLE_tolua_general_Object_addState00
static int tolua_general_Object_addState00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Object",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"State",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* self = (Object*)  tolua_tousertype(tolua_S,1,0);
  State* state = ((State*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addState'",NULL);
#endif
  {
   self->addState(state);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addState'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  AnimObject */
#ifndef TOLUA_DISABLE_tolua_general_AnimObject_new00
static int tolua_general_AnimObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"AnimObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   AnimObject* tolua_ret = (AnimObject*)  new AnimObject();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"AnimObject");
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

/* method: new_local of class  AnimObject */
#ifndef TOLUA_DISABLE_tolua_general_AnimObject_new00_local
static int tolua_general_AnimObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"AnimObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   AnimObject* tolua_ret = (AnimObject*)  new AnimObject();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"AnimObject");
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

/* method: getAnimation of class  AnimObject */
#ifndef TOLUA_DISABLE_tolua_general_AnimObject_getAnimation00
static int tolua_general_AnimObject_getAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const AnimObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const AnimObject* self = (const AnimObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAnimation'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getAnimation();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAnimation of class  AnimObject */
#ifndef TOLUA_DISABLE_tolua_general_AnimObject_setAnimation00
static int tolua_general_AnimObject_setAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AnimObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AnimObject* self = (AnimObject*)  tolua_tousertype(tolua_S,1,0);
  int anim = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAnimation'",NULL);
#endif
  {
   self->setAnimation(anim);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Creature */
#ifndef TOLUA_DISABLE_tolua_general_Creature_new00
static int tolua_general_Creature_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Creature",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Creature* tolua_ret = (Creature*)  new Creature();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Creature");
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

/* method: new_local of class  Creature */
#ifndef TOLUA_DISABLE_tolua_general_Creature_new00_local
static int tolua_general_Creature_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Creature",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Creature* tolua_ret = (Creature*)  new Creature();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Creature");
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

/* method: create of class  Creature */
#ifndef TOLUA_DISABLE_tolua_general_Creature_create00
static int tolua_general_Creature_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Creature",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Creature* self = (Creature*)  tolua_tousertype(tolua_S,1,0);
  const char* filename = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(filename);
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

/* method: moveto of class  Creature */
#ifndef TOLUA_DISABLE_tolua_general_Creature_moveto00
static int tolua_general_Creature_moveto00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Creature",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Creature",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Creature* self = (Creature*)  tolua_tousertype(tolua_S,1,0);
  Creature* t = ((Creature*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveto'",NULL);
#endif
  {
   self->moveto(t);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveto'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWeapon of class  Creature */
#ifndef TOLUA_DISABLE_tolua_general_Creature_setWeapon00
static int tolua_general_Creature_setWeapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Creature",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Weapon",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Creature* self = (Creature*)  tolua_tousertype(tolua_S,1,0);
  Weapon* w = ((Weapon*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWeapon'",NULL);
#endif
  {
   self->setWeapon(w);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWeapon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWeapon of class  Creature */
#ifndef TOLUA_DISABLE_tolua_general_Creature_getWeapon00
static int tolua_general_Creature_getWeapon00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Creature",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Creature* self = (Creature*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWeapon'",NULL);
#endif
  {
   Weapon* tolua_ret = (Weapon*)  self->getWeapon();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Weapon");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWeapon'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_general_ParticleSystem_new00
static int tolua_general_ParticleSystem_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ParticleSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ParticleSystem* tolua_ret = (ParticleSystem*)  new ParticleSystem();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ParticleSystem");
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

/* method: new_local of class  ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_general_ParticleSystem_new00_local
static int tolua_general_ParticleSystem_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ParticleSystem",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ParticleSystem* tolua_ret = (ParticleSystem*)  new ParticleSystem();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ParticleSystem");
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

/* method: setEmitRate of class  ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_general_ParticleSystem_setEmitRate00
static int tolua_general_ParticleSystem_setEmitRate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ParticleSystem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ParticleSystem* self = (ParticleSystem*)  tolua_tousertype(tolua_S,1,0);
  int rate = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEmitRate'",NULL);
#endif
  {
   self->setEmitRate(rate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEmitRate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setEmitCount of class  ParticleSystem */
#ifndef TOLUA_DISABLE_tolua_general_ParticleSystem_setEmitCount00
static int tolua_general_ParticleSystem_setEmitCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ParticleSystem",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ParticleSystem* self = (ParticleSystem*)  tolua_tousertype(tolua_S,1,0);
  int count = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setEmitCount'",NULL);
#endif
  {
   self->setEmitCount(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setEmitCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Weapon */
#ifndef TOLUA_DISABLE_tolua_general_Weapon_new00
static int tolua_general_Weapon_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Weapon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* owner = ((Object*)  tolua_tousertype(tolua_S,2,0));
  {
   Weapon* tolua_ret = (Weapon*)  new Weapon(owner);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Weapon");
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

/* method: new_local of class  Weapon */
#ifndef TOLUA_DISABLE_tolua_general_Weapon_new00_local
static int tolua_general_Weapon_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Weapon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Object* owner = ((Object*)  tolua_tousertype(tolua_S,2,0));
  {
   Weapon* tolua_ret = (Weapon*)  new Weapon(owner);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Weapon");
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

/* method: fireAt of class  Weapon */
#ifndef TOLUA_DISABLE_tolua_general_Weapon_fireAt00
static int tolua_general_Weapon_fireAt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Weapon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Weapon* self = (Weapon*)  tolua_tousertype(tolua_S,1,0);
  Object* target = ((Object*)  tolua_tousertype(tolua_S,2,0));
  int duration = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fireAt'",NULL);
#endif
  {
   self->fireAt(target,duration);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fireAt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setOwner of class  Weapon */
#ifndef TOLUA_DISABLE_tolua_general_Weapon_setOwner00
static int tolua_general_Weapon_setOwner00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Weapon",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Weapon* self = (Weapon*)  tolua_tousertype(tolua_S,1,0);
  Object* owner = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setOwner'",NULL);
#endif
  {
   self->setOwner(owner);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setOwner'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_create00
static int tolua_general_Layer_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
  int width = ((int)  tolua_tonumber(tolua_S,3,0));
  int height = ((int)  tolua_tonumber(tolua_S,4,0));
  const std::string effect = ((const std::string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(name,width,height,effect);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)name);
   tolua_pushcppstring(tolua_S,(const char*)effect);
  }
 }
 return 3;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getName of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_getName00
static int tolua_general_Layer_getName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Layer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Layer* self = (const Layer*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getTile of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_getTile00
static int tolua_general_Layer_getTile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTile'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getTile(x,y);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTile of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_setTile00
static int tolua_general_Layer_setTile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
  int tile = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTile'",NULL);
#endif
  {
   self->setTile(x,y,tile);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDimensions of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_getDimensions00
static int tolua_general_Layer_getDimensions00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDimensions'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->getDimensions();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDimensions'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScroll of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_getScroll00
static int tolua_general_Layer_getScroll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScroll'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->getScroll();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScroll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScrollSpeed of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_getScrollSpeed00
static int tolua_general_Layer_getScrollSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Layer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Layer* self = (const Layer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScrollSpeed'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->getScrollSpeed();
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScrollSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: screenToLayer of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_screenToLayer00
static int tolua_general_Layer_screenToLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  const Vector* v = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'screenToLayer'",NULL);
#endif
  {
   Vector tolua_ret = (Vector)  self->screenToLayer(*v);
   {
#ifdef __cplusplus
    void* tolua_obj = new Vector(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Vector));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"Vector");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'screenToLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: enableTileAnimation of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_enableTileAnimation00
static int tolua_general_Layer_enableTileAnimation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  bool enable = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'enableTileAnimation'",NULL);
#endif
  {
   self->enableTileAnimation(enable);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'enableTileAnimation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: resize of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_resize00
static int tolua_general_Layer_resize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  int newwidth = ((int)  tolua_tonumber(tolua_S,2,0));
  int newheight = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'resize'",NULL);
#endif
  {
   self->resize(newwidth,newheight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScrollSpeed of class  Layer */
#ifndef TOLUA_DISABLE_tolua_general_Layer_setScrollSpeed00
static int tolua_general_Layer_setScrollSpeed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Layer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Layer* self = (Layer*)  tolua_tousertype(tolua_S,1,0);
  float newx = ((float)  tolua_tonumber(tolua_S,2,0));
  float newy = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScrollSpeed'",NULL);
#endif
  {
   self->setScrollSpeed(newx,newy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScrollSpeed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TopDownLayer */
#ifndef TOLUA_DISABLE_tolua_general_TopDownLayer_new00
static int tolua_general_TopDownLayer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TopDownLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TopDownLayer* tolua_ret = (TopDownLayer*)  new TopDownLayer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"TopDownLayer");
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

/* method: new_local of class  TopDownLayer */
#ifndef TOLUA_DISABLE_tolua_general_TopDownLayer_new00_local
static int tolua_general_TopDownLayer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TopDownLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   TopDownLayer* tolua_ret = (TopDownLayer*)  new TopDownLayer();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"TopDownLayer");
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

/* method: new of class  IsoDiamondLayer */
#ifndef TOLUA_DISABLE_tolua_general_IsoDiamondLayer_new00
static int tolua_general_IsoDiamondLayer_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"IsoDiamondLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   IsoDiamondLayer* tolua_ret = (IsoDiamondLayer*)  new IsoDiamondLayer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"IsoDiamondLayer");
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

/* method: new_local of class  IsoDiamondLayer */
#ifndef TOLUA_DISABLE_tolua_general_IsoDiamondLayer_new00_local
static int tolua_general_IsoDiamondLayer_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"IsoDiamondLayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   IsoDiamondLayer* tolua_ret = (IsoDiamondLayer*)  new IsoDiamondLayer();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"IsoDiamondLayer");
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

/* method: new of class  Bound */
#ifndef TOLUA_DISABLE_tolua_general_Bound_new00
static int tolua_general_Bound_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Bound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Bound* tolua_ret = (Bound*)  new Bound();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Bound");
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

/* method: new_local of class  Bound */
#ifndef TOLUA_DISABLE_tolua_general_Bound_new00_local
static int tolua_general_Bound_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Bound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Bound* tolua_ret = (Bound*)  new Bound();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"Bound");
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

/* method: move of class  Bound */
#ifndef TOLUA_DISABLE_tolua_general_Bound_move00
static int tolua_general_Bound_move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Bound",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Bound* self = (Bound*)  tolua_tousertype(tolua_S,1,0);
  const Vector* offset = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'move'",NULL);
#endif
  {
   self->move(*offset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findPoint of class  Bound */
#ifndef TOLUA_DISABLE_tolua_general_Bound_findPoint00
static int tolua_general_Bound_findPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Bound",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Bound* self = (Bound*)  tolua_tousertype(tolua_S,1,0);
  const Vector* point = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findPoint'",NULL);
#endif
  {
   int tolua_ret = (int)  self->findPoint(*point);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: movePoint of class  Bound */
#ifndef TOLUA_DISABLE_tolua_general_Bound_movePoint00
static int tolua_general_Bound_movePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Bound",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Bound* self = (Bound*)  tolua_tousertype(tolua_S,1,0);
  int point = ((int)  tolua_tonumber(tolua_S,2,0));
  const Vector* offset = ((const Vector*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'movePoint'",NULL);
#endif
  {
   self->movePoint(point,*offset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'movePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_new00
static int tolua_general_World_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   World* tolua_ret = (World*)  new World();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"World");
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

/* method: new_local of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_new00_local
static int tolua_general_World_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   World* tolua_ret = (World*)  new World();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"World");
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

/* method: delete of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_delete00
static int tolua_general_World_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
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

/* method: createLayer of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_createLayer00
static int tolua_general_World_createLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createLayer'",NULL);
#endif
  {
   Layer* tolua_ret = (Layer*)  self->createLayer();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Layer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addLayer of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_addLayer00
static int tolua_general_World_addLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Layer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  Layer* player = ((Layer*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addLayer'",NULL);
#endif
  {
   int tolua_ret = (int)  self->addLayer(player);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveLayer of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_moveLayer00
static int tolua_general_World_moveLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Layer",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  Layer* player = ((Layer*)  tolua_tousertype(tolua_S,2,0));
  int offset = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveLayer'",NULL);
#endif
  {
   self->moveLayer(player,offset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLayerCount of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_getLayerCount00
static int tolua_general_World_getLayerCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const World* self = (const World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLayerCount'",NULL);
#endif
  {
   int tolua_ret = (int)  self->getLayerCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLayerCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLayer of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_getLayer00
static int tolua_general_World_getLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLayer'",NULL);
#endif
  {
   Layer* tolua_ret = (Layer*)  self->getLayer(index);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Layer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLayerType of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setLayerType00
static int tolua_general_World_setLayerType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  int type = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLayerType'",NULL);
#endif
  {
   self->setLayerType(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLayerType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addBound of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_addBound00
static int tolua_general_World_addBound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Vector",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"const Vector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  const Vector* p1 = ((const Vector*)  tolua_tousertype(tolua_S,2,0));
  const Vector* p2 = ((const Vector*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addBound'",NULL);
#endif
  {
   Bound& tolua_ret = (Bound&)  self->addBound(*p1,*p2);
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Bound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addBound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeBound of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_removeBound00
static int tolua_general_World_removeBound00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Bound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  Bound* bound = ((Bound*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeBound'",NULL);
#endif
  {
   self->removeBound(*bound);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeBound'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: collide of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_collide00
static int tolua_general_World_collide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  Object* object = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'collide'",NULL);
#endif
  {
   self->collide(*object);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'collide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_create00
static int tolua_general_World_create00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  const char* filename = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->create(filename);
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

/* method: createEmpty of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_createEmpty00
static int tolua_general_World_createEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  const std::string name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'createEmpty'",NULL);
#endif
  {
   self->createEmpty(name);
   tolua_pushcppstring(tolua_S,(const char*)name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'createEmpty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: destroy of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_destroy00
static int tolua_general_World_destroy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
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

/* method: save of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_save00
static int tolua_general_World_save00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'save'",NULL);
#endif
  {
   self->save();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: loadObjects of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_loadObjects00
static int tolua_general_World_loadObjects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  const char* filename = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'loadObjects'",NULL);
#endif
  {
   self->loadObjects(filename);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'loadObjects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAutoFollow of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setAutoFollow00
static int tolua_general_World_setAutoFollow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  bool enabled = ((bool)  tolua_toboolean(tolua_S,2,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoFollow'",NULL);
#endif
  {
   self->setAutoFollow(enabled);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoFollow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScrollMode of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setScrollMode00
static int tolua_general_World_setScrollMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  int fm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScrollMode'",NULL);
#endif
  {
   self->setScrollMode(fm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScrollMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFollowObject of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setFollowObject00
static int tolua_general_World_setFollowObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  Object* obj = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFollowObject'",NULL);
#endif
  {
   self->setFollowObject(obj);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFollowObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFollowBorderWidth of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setFollowBorderWidth00
static int tolua_general_World_setFollowBorderWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  int width = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFollowBorderWidth'",NULL);
#endif
  {
   self->setFollowBorderWidth(width);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFollowBorderWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFollowBorders of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setFollowBorders00
static int tolua_general_World_setFollowBorders00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
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
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  int left = ((int)  tolua_tonumber(tolua_S,2,0));
  int right = ((int)  tolua_tonumber(tolua_S,3,0));
  int top = ((int)  tolua_tonumber(tolua_S,4,0));
  int bottom = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFollowBorders'",NULL);
#endif
  {
   self->setFollowBorders(left,right,top,bottom);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFollowBorders'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setObjectLayer of class  World */
#ifndef TOLUA_DISABLE_tolua_general_World_setObjectLayer00
static int tolua_general_World_setObjectLayer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"World",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  World* self = (World*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setObjectLayer'",NULL);
#endif
  {
   self->setObjectLayer(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setObjectLayer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setWorld of class  SceneGraph */
#ifndef TOLUA_DISABLE_tolua_general_SceneGraph_setWorld00
static int tolua_general_SceneGraph_setWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneGraph",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"World",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneGraph* self = (SceneGraph*)  tolua_tousertype(tolua_S,1,0);
  World* world = ((World*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setWorld'",NULL);
#endif
  {
   self->setWorld(world);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setControler of class  SceneGraph */
#ifndef TOLUA_DISABLE_tolua_general_SceneGraph_setControler00
static int tolua_general_SceneGraph_setControler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneGraph",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Object",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneGraph* self = (SceneGraph*)  tolua_tousertype(tolua_S,1,0);
  Object* controler = ((Object*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setControler'",NULL);
#endif
  {
   self->setControler(controler);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setControler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getWorld of class  SceneGraph */
#ifndef TOLUA_DISABLE_tolua_general_SceneGraph_getWorld00
static int tolua_general_SceneGraph_getWorld00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneGraph",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneGraph* self = (SceneGraph*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getWorld'",NULL);
#endif
  {
   World* tolua_ret = (World*)  self->getWorld();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"World");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getWorld'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getControler of class  SceneGraph */
#ifndef TOLUA_DISABLE_tolua_general_SceneGraph_getControler00
static int tolua_general_SceneGraph_getControler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneGraph",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneGraph* self = (SceneGraph*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getControler'",NULL);
#endif
  {
   Object* tolua_ret = (Object*)  self->getControler();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Object");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getControler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAll of class  SceneGraph */
#ifndef TOLUA_DISABLE_tolua_general_SceneGraph_removeAll00
static int tolua_general_SceneGraph_removeAll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneGraph",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneGraph* self = (SceneGraph*)  tolua_tousertype(tolua_S,1,0);
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

/* method: find of class  SceneGraph */
#ifndef TOLUA_DISABLE_tolua_general_SceneGraph_find00
static int tolua_general_SceneGraph_find00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SceneGraph",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SceneGraph* self = (SceneGraph*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'find'",NULL);
#endif
  {
   SceneObject* tolua_ret = (SceneObject*)  self->find(name);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"SceneObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: print of class  Console */
#ifndef TOLUA_DISABLE_tolua_general_Console_print00
static int tolua_general_Console_print00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Console",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Console* self = (Console*)  tolua_tousertype(tolua_S,1,0);
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'print'",NULL);
#endif
  {
   self->print(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'print'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: display of class  Console */
#ifndef TOLUA_DISABLE_tolua_general_Console_display00
static int tolua_general_Console_display00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Console",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Console* self = (Console*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'display'",NULL);
#endif
  {
   self->display();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'display'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ActionMap */
#ifndef TOLUA_DISABLE_tolua_general_ActionMap_new00
static int tolua_general_ActionMap_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ActionMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ActionMap* tolua_ret = (ActionMap*)  new ActionMap();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"ActionMap");
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

/* method: new_local of class  ActionMap */
#ifndef TOLUA_DISABLE_tolua_general_ActionMap_new00_local
static int tolua_general_ActionMap_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ActionMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ActionMap* tolua_ret = (ActionMap*)  new ActionMap();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"ActionMap");
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

/* method: bind of class  ActionMap */
#ifndef TOLUA_DISABLE_tolua_general_ActionMap_bind00
static int tolua_general_ActionMap_bind00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActionMap",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActionMap* self = (ActionMap*)  tolua_tousertype(tolua_S,1,0);
  int key = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* function = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bind'",NULL);
#endif
  {
   self->bind(key,function);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bind'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  KeyMap */
#ifndef TOLUA_DISABLE_tolua_general_KeyMap_new00
static int tolua_general_KeyMap_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"KeyMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   KeyMap* tolua_ret = (KeyMap*)  new KeyMap();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"KeyMap");
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

/* method: new_local of class  KeyMap */
#ifndef TOLUA_DISABLE_tolua_general_KeyMap_new00_local
static int tolua_general_KeyMap_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"KeyMap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   KeyMap* tolua_ret = (KeyMap*)  new KeyMap();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"KeyMap");
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

/* method: bind of class  KeyMap */
#ifndef TOLUA_DISABLE_tolua_general_KeyMap_bind00
static int tolua_general_KeyMap_bind00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"KeyMap",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  KeyMap* self = (KeyMap*)  tolua_tousertype(tolua_S,1,0);
  int key = ((int)  tolua_tonumber(tolua_S,2,0));
  int action = ((int)  tolua_tonumber(tolua_S,3,0));
  bool local = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bind'",NULL);
#endif
  {
   self->bind(key,action,local);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bind'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: play of class  Sound */
#ifndef TOLUA_DISABLE_tolua_general_Sound_play00
static int tolua_general_Sound_play00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Sound",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Sound* self = (Sound*)  tolua_tousertype(tolua_S,1,0);
  bool loop = ((bool)  tolua_toboolean(tolua_S,2,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'play'",NULL);
#endif
  {
   self->play(loop);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'play'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stop of class  Sound */
#ifndef TOLUA_DISABLE_tolua_general_Sound_stop00
static int tolua_general_Sound_stop00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Sound",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Sound* self = (Sound*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stop'",NULL);
#endif
  {
   self->stop();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stop'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: load of class  SoundManager */
#ifndef TOLUA_DISABLE_tolua_general_SoundManager_load00
static int tolua_general_SoundManager_load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"SoundManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  SoundManager* self = (SoundManager*)  tolua_tousertype(tolua_S,1,0);
  const char* file = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'load'",NULL);
#endif
  {
   Sound* tolua_ret = (Sound*)  self->load(file);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"Sound");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addRequest of class  ScriptManager */
#ifndef TOLUA_DISABLE_tolua_general_ScriptManager_addRequest00
static int tolua_general_ScriptManager_addRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptManager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptManager* self = (ScriptManager*)  tolua_tousertype(tolua_S,1,0);
  const char* cmd = ((const char*)  tolua_tostring(tolua_S,2,0));
  int type = ((int)  tolua_tonumber(tolua_S,3,0));
  int time = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addRequest'",NULL);
#endif
  {
   int tolua_ret = (int)  self->addRequest(cmd,type,time);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeRequest of class  ScriptManager */
#ifndef TOLUA_DISABLE_tolua_general_ScriptManager_removeRequest00
static int tolua_general_ScriptManager_removeRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptManager* self = (ScriptManager*)  tolua_tousertype(tolua_S,1,0);
  const int jobid = ((const int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeRequest'",NULL);
#endif
  {
   self->removeRequest(jobid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeAllRequests of class  ScriptManager */
#ifndef TOLUA_DISABLE_tolua_general_ScriptManager_removeAllRequests00
static int tolua_general_ScriptManager_removeAllRequests00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ScriptManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ScriptManager* self = (ScriptManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllRequests'",NULL);
#endif
  {
   self->removeAllRequests();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeAllRequests'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_general_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"ETopDown",ETopDown);
  tolua_constant(tolua_S,"EIsoDiamond",EIsoDiamond);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Vector","Vector","",tolua_collect_Vector);
  #else
  tolua_cclass(tolua_S,"Vector","Vector","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Vector");
   tolua_function(tolua_S,"new",tolua_general_Vector_new00);
   tolua_function(tolua_S,"new_local",tolua_general_Vector_new00_local);
   tolua_function(tolua_S,".call",tolua_general_Vector_new00_local);
   tolua_function(tolua_S,"new",tolua_general_Vector_new01);
   tolua_function(tolua_S,"new_local",tolua_general_Vector_new01_local);
   tolua_function(tolua_S,".call",tolua_general_Vector_new01_local);
   tolua_function(tolua_S,"normalize",tolua_general_Vector_normalize00);
   tolua_function(tolua_S,"length",tolua_general_Vector_length00);
   tolua_function(tolua_S,"dot",tolua_general_Vector_dot00);
   tolua_function(tolua_S,"set",tolua_general_Vector_set00);
   tolua_function(tolua_S,".add",tolua_general_Vector__add00);
   tolua_function(tolua_S,".sub",tolua_general_Vector__sub00);
   tolua_variable(tolua_S,"x",tolua_get_Vector_x,tolua_set_Vector_x);
   tolua_variable(tolua_S,"y",tolua_get_Vector_y,tolua_set_Vector_y);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Color","Color","",tolua_collect_Color);
  #else
  tolua_cclass(tolua_S,"Color","Color","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Color");
   tolua_function(tolua_S,"new",tolua_general_Color_new00);
   tolua_function(tolua_S,"new_local",tolua_general_Color_new00_local);
   tolua_function(tolua_S,".call",tolua_general_Color_new00_local);
   tolua_function(tolua_S,"new",tolua_general_Color_new01);
   tolua_function(tolua_S,"new_local",tolua_general_Color_new01_local);
   tolua_function(tolua_S,".call",tolua_general_Color_new01_local);
   tolua_function(tolua_S,"set",tolua_general_Color_set00);
   tolua_variable(tolua_S,"r",tolua_get_Color_r,tolua_set_Color_r);
   tolua_variable(tolua_S,"g",tolua_get_Color_g,tolua_set_Color_g);
   tolua_variable(tolua_S,"b",tolua_get_Color_b,tolua_set_Color_b);
   tolua_variable(tolua_S,"a",tolua_get_Color_a,tolua_set_Color_a);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Particle","Particle","",NULL);
  tolua_beginmodule(tolua_S,"Particle");
   tolua_function(tolua_S,"setColor",tolua_general_Particle_setColor00);
   tolua_variable(tolua_S,"color",tolua_get_Particle_color,tolua_set_Particle_color);
   tolua_variable(tolua_S,"life",tolua_get_Particle_life,tolua_set_Particle_life);
   tolua_variable(tolua_S,"state",tolua_get_Particle_state,tolua_set_Particle_state);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"State","State","",NULL);
  tolua_beginmodule(tolua_S,"State");
   tolua_function(tolua_S,"duration",tolua_general_State_duration00);
   tolua_function(tolua_S,"duration",tolua_general_State_duration01);
   tolua_function(tolua_S,"start",tolua_general_State_start00);
   tolua_function(tolua_S,"start",tolua_general_State_start01);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"LocationState","LocationState","State",tolua_collect_LocationState);
  #else
  tolua_cclass(tolua_S,"LocationState","LocationState","State",NULL);
  #endif
  tolua_beginmodule(tolua_S,"LocationState");
   tolua_function(tolua_S,"new",tolua_general_LocationState_new00);
   tolua_function(tolua_S,"new_local",tolua_general_LocationState_new00_local);
   tolua_function(tolua_S,".call",tolua_general_LocationState_new00_local);
   tolua_function(tolua_S,"new",tolua_general_LocationState_new01);
   tolua_function(tolua_S,"new_local",tolua_general_LocationState_new01_local);
   tolua_function(tolua_S,".call",tolua_general_LocationState_new01_local);
   tolua_function(tolua_S,"location",tolua_general_LocationState_location00);
   tolua_function(tolua_S,"location",tolua_general_LocationState_location01);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MoveState","MoveState","State",tolua_collect_MoveState);
  #else
  tolua_cclass(tolua_S,"MoveState","MoveState","State",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MoveState");
   tolua_function(tolua_S,"new",tolua_general_MoveState_new00);
   tolua_function(tolua_S,"new_local",tolua_general_MoveState_new00_local);
   tolua_function(tolua_S,".call",tolua_general_MoveState_new00_local);
   tolua_function(tolua_S,"new",tolua_general_MoveState_new01);
   tolua_function(tolua_S,"new_local",tolua_general_MoveState_new01_local);
   tolua_function(tolua_S,".call",tolua_general_MoveState_new01_local);
   tolua_function(tolua_S,"destination",tolua_general_MoveState_destination00);
   tolua_function(tolua_S,"destination",tolua_general_MoveState_destination01);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"RotateState","RotateState","State",tolua_collect_RotateState);
  #else
  tolua_cclass(tolua_S,"RotateState","RotateState","State",NULL);
  #endif
  tolua_beginmodule(tolua_S,"RotateState");
   tolua_function(tolua_S,"new",tolua_general_RotateState_new00);
   tolua_function(tolua_S,"new_local",tolua_general_RotateState_new00_local);
   tolua_function(tolua_S,".call",tolua_general_RotateState_new00_local);
   tolua_function(tolua_S,"new",tolua_general_RotateState_new01);
   tolua_function(tolua_S,"new_local",tolua_general_RotateState_new01_local);
   tolua_function(tolua_S,".call",tolua_general_RotateState_new01_local);
   tolua_function(tolua_S,"rotation",tolua_general_RotateState_rotation00);
   tolua_function(tolua_S,"rotation",tolua_general_RotateState_rotation01);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SceneObject","SceneObject","",NULL);
  tolua_beginmodule(tolua_S,"SceneObject");
   tolua_function(tolua_S,"add",tolua_general_SceneObject_add00);
   tolua_function(tolua_S,"find",tolua_general_SceneObject_find00);
   tolua_function(tolua_S,"setName",tolua_general_SceneObject_setName00);
   tolua_function(tolua_S,"getName",tolua_general_SceneObject_getName00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Object","Object","SceneObject",tolua_collect_Object);
  #else
  tolua_cclass(tolua_S,"Object","Object","SceneObject",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Object");
   tolua_function(tolua_S,"new",tolua_general_Object_new00);
   tolua_function(tolua_S,"new_local",tolua_general_Object_new00_local);
   tolua_function(tolua_S,".call",tolua_general_Object_new00_local);
   tolua_function(tolua_S,"create",tolua_general_Object_create00);
   tolua_function(tolua_S,"flip",tolua_general_Object_flip00);
   tolua_function(tolua_S,"direction",tolua_general_Object_direction00);
   tolua_function(tolua_S,"rotate",tolua_general_Object_rotate00);
   tolua_function(tolua_S,"setPosition",tolua_general_Object_setPosition00);
   tolua_function(tolua_S,"setVelocity",tolua_general_Object_setVelocity00);
   tolua_function(tolua_S,"setRotation",tolua_general_Object_setRotation00);
   tolua_function(tolua_S,"setMoveSpeed",tolua_general_Object_setMoveSpeed00);
   tolua_function(tolua_S,"getPosition",tolua_general_Object_getPosition00);
   tolua_function(tolua_S,"getVelocity",tolua_general_Object_getVelocity00);
   tolua_function(tolua_S,"getRotation",tolua_general_Object_getRotation00);
   tolua_function(tolua_S,"addState",tolua_general_Object_addState00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"AnimObject","AnimObject","Object",tolua_collect_AnimObject);
  #else
  tolua_cclass(tolua_S,"AnimObject","AnimObject","Object",NULL);
  #endif
  tolua_beginmodule(tolua_S,"AnimObject");
   tolua_function(tolua_S,"new",tolua_general_AnimObject_new00);
   tolua_function(tolua_S,"new_local",tolua_general_AnimObject_new00_local);
   tolua_function(tolua_S,".call",tolua_general_AnimObject_new00_local);
   tolua_function(tolua_S,"getAnimation",tolua_general_AnimObject_getAnimation00);
   tolua_function(tolua_S,"setAnimation",tolua_general_AnimObject_setAnimation00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Creature","Creature","AnimObject",tolua_collect_Creature);
  #else
  tolua_cclass(tolua_S,"Creature","Creature","AnimObject",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Creature");
   tolua_function(tolua_S,"new",tolua_general_Creature_new00);
   tolua_function(tolua_S,"new_local",tolua_general_Creature_new00_local);
   tolua_function(tolua_S,".call",tolua_general_Creature_new00_local);
   tolua_function(tolua_S,"create",tolua_general_Creature_create00);
   tolua_function(tolua_S,"moveto",tolua_general_Creature_moveto00);
   tolua_function(tolua_S,"setWeapon",tolua_general_Creature_setWeapon00);
   tolua_function(tolua_S,"getWeapon",tolua_general_Creature_getWeapon00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ParticleSystem","ParticleSystem","Object",tolua_collect_ParticleSystem);
  #else
  tolua_cclass(tolua_S,"ParticleSystem","ParticleSystem","Object",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ParticleSystem");
   tolua_function(tolua_S,"new",tolua_general_ParticleSystem_new00);
   tolua_function(tolua_S,"new_local",tolua_general_ParticleSystem_new00_local);
   tolua_function(tolua_S,".call",tolua_general_ParticleSystem_new00_local);
   tolua_function(tolua_S,"setEmitRate",tolua_general_ParticleSystem_setEmitRate00);
   tolua_function(tolua_S,"setEmitCount",tolua_general_ParticleSystem_setEmitCount00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Weapon","Weapon","",tolua_collect_Weapon);
  #else
  tolua_cclass(tolua_S,"Weapon","Weapon","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Weapon");
   tolua_function(tolua_S,"new",tolua_general_Weapon_new00);
   tolua_function(tolua_S,"new_local",tolua_general_Weapon_new00_local);
   tolua_function(tolua_S,".call",tolua_general_Weapon_new00_local);
   tolua_function(tolua_S,"fireAt",tolua_general_Weapon_fireAt00);
   tolua_function(tolua_S,"setOwner",tolua_general_Weapon_setOwner00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Layer","Layer","",NULL);
  tolua_beginmodule(tolua_S,"Layer");
   tolua_function(tolua_S,"create",tolua_general_Layer_create00);
   tolua_function(tolua_S,"getName",tolua_general_Layer_getName00);
   tolua_function(tolua_S,"getTile",tolua_general_Layer_getTile00);
   tolua_function(tolua_S,"setTile",tolua_general_Layer_setTile00);
   tolua_function(tolua_S,"getDimensions",tolua_general_Layer_getDimensions00);
   tolua_function(tolua_S,"getScroll",tolua_general_Layer_getScroll00);
   tolua_function(tolua_S,"getScrollSpeed",tolua_general_Layer_getScrollSpeed00);
   tolua_function(tolua_S,"screenToLayer",tolua_general_Layer_screenToLayer00);
   tolua_function(tolua_S,"enableTileAnimation",tolua_general_Layer_enableTileAnimation00);
   tolua_function(tolua_S,"resize",tolua_general_Layer_resize00);
   tolua_function(tolua_S,"setScrollSpeed",tolua_general_Layer_setScrollSpeed00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"TopDownLayer","TopDownLayer","Layer",tolua_collect_TopDownLayer);
  #else
  tolua_cclass(tolua_S,"TopDownLayer","TopDownLayer","Layer",NULL);
  #endif
  tolua_beginmodule(tolua_S,"TopDownLayer");
   tolua_function(tolua_S,"new",tolua_general_TopDownLayer_new00);
   tolua_function(tolua_S,"new_local",tolua_general_TopDownLayer_new00_local);
   tolua_function(tolua_S,".call",tolua_general_TopDownLayer_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"IsoDiamondLayer","IsoDiamondLayer","Layer",tolua_collect_IsoDiamondLayer);
  #else
  tolua_cclass(tolua_S,"IsoDiamondLayer","IsoDiamondLayer","Layer",NULL);
  #endif
  tolua_beginmodule(tolua_S,"IsoDiamondLayer");
   tolua_function(tolua_S,"new",tolua_general_IsoDiamondLayer_new00);
   tolua_function(tolua_S,"new_local",tolua_general_IsoDiamondLayer_new00_local);
   tolua_function(tolua_S,".call",tolua_general_IsoDiamondLayer_new00_local);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Bound","Bound","",tolua_collect_Bound);
  #else
  tolua_cclass(tolua_S,"Bound","Bound","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Bound");
   tolua_function(tolua_S,"new",tolua_general_Bound_new00);
   tolua_function(tolua_S,"new_local",tolua_general_Bound_new00_local);
   tolua_function(tolua_S,".call",tolua_general_Bound_new00_local);
   tolua_function(tolua_S,"move",tolua_general_Bound_move00);
   tolua_function(tolua_S,"findPoint",tolua_general_Bound_findPoint00);
   tolua_function(tolua_S,"movePoint",tolua_general_Bound_movePoint00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"World","World","SceneObject",tolua_collect_World);
  #else
  tolua_cclass(tolua_S,"World","World","SceneObject",NULL);
  #endif
  tolua_beginmodule(tolua_S,"World");
   tolua_function(tolua_S,"new",tolua_general_World_new00);
   tolua_function(tolua_S,"new_local",tolua_general_World_new00_local);
   tolua_function(tolua_S,".call",tolua_general_World_new00_local);
   tolua_function(tolua_S,"delete",tolua_general_World_delete00);
   tolua_constant(tolua_S,"FollowObject",World::FollowObject);
   tolua_constant(tolua_S,"FollowMouse",World::FollowMouse);
   tolua_constant(tolua_S,"NoFollow",World::NoFollow);
   tolua_function(tolua_S,"createLayer",tolua_general_World_createLayer00);
   tolua_function(tolua_S,"addLayer",tolua_general_World_addLayer00);
   tolua_function(tolua_S,"moveLayer",tolua_general_World_moveLayer00);
   tolua_function(tolua_S,"getLayerCount",tolua_general_World_getLayerCount00);
   tolua_function(tolua_S,"getLayer",tolua_general_World_getLayer00);
   tolua_function(tolua_S,"setLayerType",tolua_general_World_setLayerType00);
   tolua_function(tolua_S,"addBound",tolua_general_World_addBound00);
   tolua_function(tolua_S,"removeBound",tolua_general_World_removeBound00);
   tolua_function(tolua_S,"collide",tolua_general_World_collide00);
   tolua_function(tolua_S,"create",tolua_general_World_create00);
   tolua_function(tolua_S,"createEmpty",tolua_general_World_createEmpty00);
   tolua_function(tolua_S,"destroy",tolua_general_World_destroy00);
   tolua_function(tolua_S,"save",tolua_general_World_save00);
   tolua_function(tolua_S,"loadObjects",tolua_general_World_loadObjects00);
   tolua_function(tolua_S,"setAutoFollow",tolua_general_World_setAutoFollow00);
   tolua_function(tolua_S,"setScrollMode",tolua_general_World_setScrollMode00);
   tolua_function(tolua_S,"setFollowObject",tolua_general_World_setFollowObject00);
   tolua_function(tolua_S,"setFollowBorderWidth",tolua_general_World_setFollowBorderWidth00);
   tolua_function(tolua_S,"setFollowBorders",tolua_general_World_setFollowBorders00);
   tolua_function(tolua_S,"setObjectLayer",tolua_general_World_setObjectLayer00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SceneGraph","SceneGraph","",NULL);
  tolua_beginmodule(tolua_S,"SceneGraph");
   tolua_function(tolua_S,"setWorld",tolua_general_SceneGraph_setWorld00);
   tolua_function(tolua_S,"setControler",tolua_general_SceneGraph_setControler00);
   tolua_function(tolua_S,"getWorld",tolua_general_SceneGraph_getWorld00);
   tolua_function(tolua_S,"getControler",tolua_general_SceneGraph_getControler00);
   tolua_function(tolua_S,"removeAll",tolua_general_SceneGraph_removeAll00);
   tolua_function(tolua_S,"find",tolua_general_SceneGraph_find00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Console","Console","",NULL);
  tolua_beginmodule(tolua_S,"Console");
   tolua_function(tolua_S,"print",tolua_general_Console_print00);
   tolua_function(tolua_S,"display",tolua_general_Console_display00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ActionMap","ActionMap","",tolua_collect_ActionMap);
  #else
  tolua_cclass(tolua_S,"ActionMap","ActionMap","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ActionMap");
   tolua_function(tolua_S,"new",tolua_general_ActionMap_new00);
   tolua_function(tolua_S,"new_local",tolua_general_ActionMap_new00_local);
   tolua_function(tolua_S,".call",tolua_general_ActionMap_new00_local);
   tolua_function(tolua_S,"bind",tolua_general_ActionMap_bind00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"KeyMap","KeyMap","",tolua_collect_KeyMap);
  #else
  tolua_cclass(tolua_S,"KeyMap","KeyMap","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"KeyMap");
   tolua_function(tolua_S,"new",tolua_general_KeyMap_new00);
   tolua_function(tolua_S,"new_local",tolua_general_KeyMap_new00_local);
   tolua_function(tolua_S,".call",tolua_general_KeyMap_new00_local);
   tolua_function(tolua_S,"bind",tolua_general_KeyMap_bind00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Sound","Sound","",NULL);
  tolua_beginmodule(tolua_S,"Sound");
   tolua_function(tolua_S,"play",tolua_general_Sound_play00);
   tolua_function(tolua_S,"stop",tolua_general_Sound_stop00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"SoundManager","SoundManager","",NULL);
  tolua_beginmodule(tolua_S,"SoundManager");
   tolua_function(tolua_S,"load",tolua_general_SoundManager_load00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ScriptManager","ScriptManager","",NULL);
  tolua_beginmodule(tolua_S,"ScriptManager");
   tolua_function(tolua_S,"addRequest",tolua_general_ScriptManager_addRequest00);
   tolua_function(tolua_S,"removeRequest",tolua_general_ScriptManager_removeRequest00);
   tolua_function(tolua_S,"removeAllRequests",tolua_general_ScriptManager_removeAllRequests00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"moveLeft",moveLeft);
  tolua_constant(tolua_S,"moveRight",moveRight);
  tolua_constant(tolua_S,"moveUp",moveUp);
  tolua_constant(tolua_S,"moveDown",moveDown);
  tolua_constant(tolua_S,"jump",jump);
  tolua_constant(tolua_S,"swapConsole",swapConsole);
  tolua_constant(tolua_S,"quit",quit);
  tolua_constant(tolua_S,"SDLK_UNKNOWN",SDLK_UNKNOWN);
  tolua_constant(tolua_S,"SDLK_FIRST",SDLK_FIRST);
  tolua_constant(tolua_S,"SDLK_BACKSPACE",SDLK_BACKSPACE);
  tolua_constant(tolua_S,"SDLK_TAB",SDLK_TAB);
  tolua_constant(tolua_S,"SDLK_CLEAR",SDLK_CLEAR);
  tolua_constant(tolua_S,"SDLK_RETURN",SDLK_RETURN);
  tolua_constant(tolua_S,"SDLK_PAUSE",SDLK_PAUSE);
  tolua_constant(tolua_S,"SDLK_ESCAPE",SDLK_ESCAPE);
  tolua_constant(tolua_S,"SDLK_SPACE",SDLK_SPACE);
  tolua_constant(tolua_S,"SDLK_EXCLAIM",SDLK_EXCLAIM);
  tolua_constant(tolua_S,"SDLK_QUOTEDBL",SDLK_QUOTEDBL);
  tolua_constant(tolua_S,"SDLK_HASH",SDLK_HASH);
  tolua_constant(tolua_S,"SDLK_DOLLAR",SDLK_DOLLAR);
  tolua_constant(tolua_S,"SDLK_AMPERSAND",SDLK_AMPERSAND);
  tolua_constant(tolua_S,"SDLK_QUOTE",SDLK_QUOTE);
  tolua_constant(tolua_S,"SDLK_LEFTPAREN",SDLK_LEFTPAREN);
  tolua_constant(tolua_S,"SDLK_RIGHTPAREN",SDLK_RIGHTPAREN);
  tolua_constant(tolua_S,"SDLK_ASTERISK",SDLK_ASTERISK);
  tolua_constant(tolua_S,"SDLK_PLUS",SDLK_PLUS);
  tolua_constant(tolua_S,"SDLK_COMMA",SDLK_COMMA);
  tolua_constant(tolua_S,"SDLK_MINUS",SDLK_MINUS);
  tolua_constant(tolua_S,"SDLK_PERIOD",SDLK_PERIOD);
  tolua_constant(tolua_S,"SDLK_SLASH",SDLK_SLASH);
  tolua_constant(tolua_S,"SDLK_0",SDLK_0);
  tolua_constant(tolua_S,"SDLK_1",SDLK_1);
  tolua_constant(tolua_S,"SDLK_2",SDLK_2);
  tolua_constant(tolua_S,"SDLK_3",SDLK_3);
  tolua_constant(tolua_S,"SDLK_4",SDLK_4);
  tolua_constant(tolua_S,"SDLK_5",SDLK_5);
  tolua_constant(tolua_S,"SDLK_6",SDLK_6);
  tolua_constant(tolua_S,"SDLK_7",SDLK_7);
  tolua_constant(tolua_S,"SDLK_8",SDLK_8);
  tolua_constant(tolua_S,"SDLK_9",SDLK_9);
  tolua_constant(tolua_S,"SDLK_COLON",SDLK_COLON);
  tolua_constant(tolua_S,"SDLK_SEMICOLON",SDLK_SEMICOLON);
  tolua_constant(tolua_S,"SDLK_LESS",SDLK_LESS);
  tolua_constant(tolua_S,"SDLK_EQUALS",SDLK_EQUALS);
  tolua_constant(tolua_S,"SDLK_GREATER",SDLK_GREATER);
  tolua_constant(tolua_S,"SDLK_QUESTION",SDLK_QUESTION);
  tolua_constant(tolua_S,"SDLK_AT",SDLK_AT);
  tolua_constant(tolua_S,"SDLK_LEFTBRACKET",SDLK_LEFTBRACKET);
  tolua_constant(tolua_S,"SDLK_BACKSLASH",SDLK_BACKSLASH);
  tolua_constant(tolua_S,"SDLK_RIGHTBRACKET",SDLK_RIGHTBRACKET);
  tolua_constant(tolua_S,"SDLK_CARET",SDLK_CARET);
  tolua_constant(tolua_S,"SDLK_UNDERSCORE",SDLK_UNDERSCORE);
  tolua_constant(tolua_S,"SDLK_BACKQUOTE",SDLK_BACKQUOTE);
  tolua_constant(tolua_S,"SDLK_a",SDLK_a);
  tolua_constant(tolua_S,"SDLK_b",SDLK_b);
  tolua_constant(tolua_S,"SDLK_c",SDLK_c);
  tolua_constant(tolua_S,"SDLK_d",SDLK_d);
  tolua_constant(tolua_S,"SDLK_e",SDLK_e);
  tolua_constant(tolua_S,"SDLK_f",SDLK_f);
  tolua_constant(tolua_S,"SDLK_g",SDLK_g);
  tolua_constant(tolua_S,"SDLK_h",SDLK_h);
  tolua_constant(tolua_S,"SDLK_i",SDLK_i);
  tolua_constant(tolua_S,"SDLK_j",SDLK_j);
  tolua_constant(tolua_S,"SDLK_k",SDLK_k);
  tolua_constant(tolua_S,"SDLK_l",SDLK_l);
  tolua_constant(tolua_S,"SDLK_m",SDLK_m);
  tolua_constant(tolua_S,"SDLK_n",SDLK_n);
  tolua_constant(tolua_S,"SDLK_o",SDLK_o);
  tolua_constant(tolua_S,"SDLK_p",SDLK_p);
  tolua_constant(tolua_S,"SDLK_q",SDLK_q);
  tolua_constant(tolua_S,"SDLK_r",SDLK_r);
  tolua_constant(tolua_S,"SDLK_s",SDLK_s);
  tolua_constant(tolua_S,"SDLK_t",SDLK_t);
  tolua_constant(tolua_S,"SDLK_u",SDLK_u);
  tolua_constant(tolua_S,"SDLK_v",SDLK_v);
  tolua_constant(tolua_S,"SDLK_w",SDLK_w);
  tolua_constant(tolua_S,"SDLK_x",SDLK_x);
  tolua_constant(tolua_S,"SDLK_y",SDLK_y);
  tolua_constant(tolua_S,"SDLK_z",SDLK_z);
  tolua_constant(tolua_S,"SDLK_DELETE",SDLK_DELETE);
  tolua_constant(tolua_S,"SDLK_WORLD_0",SDLK_WORLD_0);
  tolua_constant(tolua_S,"SDLK_WORLD_1",SDLK_WORLD_1);
  tolua_constant(tolua_S,"SDLK_WORLD_2",SDLK_WORLD_2);
  tolua_constant(tolua_S,"SDLK_WORLD_3",SDLK_WORLD_3);
  tolua_constant(tolua_S,"SDLK_WORLD_4",SDLK_WORLD_4);
  tolua_constant(tolua_S,"SDLK_WORLD_5",SDLK_WORLD_5);
  tolua_constant(tolua_S,"SDLK_WORLD_6",SDLK_WORLD_6);
  tolua_constant(tolua_S,"SDLK_WORLD_7",SDLK_WORLD_7);
  tolua_constant(tolua_S,"SDLK_WORLD_8",SDLK_WORLD_8);
  tolua_constant(tolua_S,"SDLK_WORLD_9",SDLK_WORLD_9);
  tolua_constant(tolua_S,"SDLK_WORLD_10",SDLK_WORLD_10);
  tolua_constant(tolua_S,"SDLK_WORLD_11",SDLK_WORLD_11);
  tolua_constant(tolua_S,"SDLK_WORLD_12",SDLK_WORLD_12);
  tolua_constant(tolua_S,"SDLK_WORLD_13",SDLK_WORLD_13);
  tolua_constant(tolua_S,"SDLK_WORLD_14",SDLK_WORLD_14);
  tolua_constant(tolua_S,"SDLK_WORLD_15",SDLK_WORLD_15);
  tolua_constant(tolua_S,"SDLK_WORLD_16",SDLK_WORLD_16);
  tolua_constant(tolua_S,"SDLK_WORLD_17",SDLK_WORLD_17);
  tolua_constant(tolua_S,"SDLK_WORLD_18",SDLK_WORLD_18);
  tolua_constant(tolua_S,"SDLK_WORLD_19",SDLK_WORLD_19);
  tolua_constant(tolua_S,"SDLK_WORLD_20",SDLK_WORLD_20);
  tolua_constant(tolua_S,"SDLK_WORLD_21",SDLK_WORLD_21);
  tolua_constant(tolua_S,"SDLK_WORLD_22",SDLK_WORLD_22);
  tolua_constant(tolua_S,"SDLK_WORLD_23",SDLK_WORLD_23);
  tolua_constant(tolua_S,"SDLK_WORLD_24",SDLK_WORLD_24);
  tolua_constant(tolua_S,"SDLK_WORLD_25",SDLK_WORLD_25);
  tolua_constant(tolua_S,"SDLK_WORLD_26",SDLK_WORLD_26);
  tolua_constant(tolua_S,"SDLK_WORLD_27",SDLK_WORLD_27);
  tolua_constant(tolua_S,"SDLK_WORLD_28",SDLK_WORLD_28);
  tolua_constant(tolua_S,"SDLK_WORLD_29",SDLK_WORLD_29);
  tolua_constant(tolua_S,"SDLK_WORLD_30",SDLK_WORLD_30);
  tolua_constant(tolua_S,"SDLK_WORLD_31",SDLK_WORLD_31);
  tolua_constant(tolua_S,"SDLK_WORLD_32",SDLK_WORLD_32);
  tolua_constant(tolua_S,"SDLK_WORLD_33",SDLK_WORLD_33);
  tolua_constant(tolua_S,"SDLK_WORLD_34",SDLK_WORLD_34);
  tolua_constant(tolua_S,"SDLK_WORLD_35",SDLK_WORLD_35);
  tolua_constant(tolua_S,"SDLK_WORLD_36",SDLK_WORLD_36);
  tolua_constant(tolua_S,"SDLK_WORLD_37",SDLK_WORLD_37);
  tolua_constant(tolua_S,"SDLK_WORLD_38",SDLK_WORLD_38);
  tolua_constant(tolua_S,"SDLK_WORLD_39",SDLK_WORLD_39);
  tolua_constant(tolua_S,"SDLK_WORLD_40",SDLK_WORLD_40);
  tolua_constant(tolua_S,"SDLK_WORLD_41",SDLK_WORLD_41);
  tolua_constant(tolua_S,"SDLK_WORLD_42",SDLK_WORLD_42);
  tolua_constant(tolua_S,"SDLK_WORLD_43",SDLK_WORLD_43);
  tolua_constant(tolua_S,"SDLK_WORLD_44",SDLK_WORLD_44);
  tolua_constant(tolua_S,"SDLK_WORLD_45",SDLK_WORLD_45);
  tolua_constant(tolua_S,"SDLK_WORLD_46",SDLK_WORLD_46);
  tolua_constant(tolua_S,"SDLK_WORLD_47",SDLK_WORLD_47);
  tolua_constant(tolua_S,"SDLK_WORLD_48",SDLK_WORLD_48);
  tolua_constant(tolua_S,"SDLK_WORLD_49",SDLK_WORLD_49);
  tolua_constant(tolua_S,"SDLK_WORLD_50",SDLK_WORLD_50);
  tolua_constant(tolua_S,"SDLK_WORLD_51",SDLK_WORLD_51);
  tolua_constant(tolua_S,"SDLK_WORLD_52",SDLK_WORLD_52);
  tolua_constant(tolua_S,"SDLK_WORLD_53",SDLK_WORLD_53);
  tolua_constant(tolua_S,"SDLK_WORLD_54",SDLK_WORLD_54);
  tolua_constant(tolua_S,"SDLK_WORLD_55",SDLK_WORLD_55);
  tolua_constant(tolua_S,"SDLK_WORLD_56",SDLK_WORLD_56);
  tolua_constant(tolua_S,"SDLK_WORLD_57",SDLK_WORLD_57);
  tolua_constant(tolua_S,"SDLK_WORLD_58",SDLK_WORLD_58);
  tolua_constant(tolua_S,"SDLK_WORLD_59",SDLK_WORLD_59);
  tolua_constant(tolua_S,"SDLK_WORLD_60",SDLK_WORLD_60);
  tolua_constant(tolua_S,"SDLK_WORLD_61",SDLK_WORLD_61);
  tolua_constant(tolua_S,"SDLK_WORLD_62",SDLK_WORLD_62);
  tolua_constant(tolua_S,"SDLK_WORLD_63",SDLK_WORLD_63);
  tolua_constant(tolua_S,"SDLK_WORLD_64",SDLK_WORLD_64);
  tolua_constant(tolua_S,"SDLK_WORLD_65",SDLK_WORLD_65);
  tolua_constant(tolua_S,"SDLK_WORLD_66",SDLK_WORLD_66);
  tolua_constant(tolua_S,"SDLK_WORLD_67",SDLK_WORLD_67);
  tolua_constant(tolua_S,"SDLK_WORLD_68",SDLK_WORLD_68);
  tolua_constant(tolua_S,"SDLK_WORLD_69",SDLK_WORLD_69);
  tolua_constant(tolua_S,"SDLK_WORLD_70",SDLK_WORLD_70);
  tolua_constant(tolua_S,"SDLK_WORLD_71",SDLK_WORLD_71);
  tolua_constant(tolua_S,"SDLK_WORLD_72",SDLK_WORLD_72);
  tolua_constant(tolua_S,"SDLK_WORLD_73",SDLK_WORLD_73);
  tolua_constant(tolua_S,"SDLK_WORLD_74",SDLK_WORLD_74);
  tolua_constant(tolua_S,"SDLK_WORLD_75",SDLK_WORLD_75);
  tolua_constant(tolua_S,"SDLK_WORLD_76",SDLK_WORLD_76);
  tolua_constant(tolua_S,"SDLK_WORLD_77",SDLK_WORLD_77);
  tolua_constant(tolua_S,"SDLK_WORLD_78",SDLK_WORLD_78);
  tolua_constant(tolua_S,"SDLK_WORLD_79",SDLK_WORLD_79);
  tolua_constant(tolua_S,"SDLK_WORLD_80",SDLK_WORLD_80);
  tolua_constant(tolua_S,"SDLK_WORLD_81",SDLK_WORLD_81);
  tolua_constant(tolua_S,"SDLK_WORLD_82",SDLK_WORLD_82);
  tolua_constant(tolua_S,"SDLK_WORLD_83",SDLK_WORLD_83);
  tolua_constant(tolua_S,"SDLK_WORLD_84",SDLK_WORLD_84);
  tolua_constant(tolua_S,"SDLK_WORLD_85",SDLK_WORLD_85);
  tolua_constant(tolua_S,"SDLK_WORLD_86",SDLK_WORLD_86);
  tolua_constant(tolua_S,"SDLK_WORLD_87",SDLK_WORLD_87);
  tolua_constant(tolua_S,"SDLK_WORLD_88",SDLK_WORLD_88);
  tolua_constant(tolua_S,"SDLK_WORLD_89",SDLK_WORLD_89);
  tolua_constant(tolua_S,"SDLK_WORLD_90",SDLK_WORLD_90);
  tolua_constant(tolua_S,"SDLK_WORLD_91",SDLK_WORLD_91);
  tolua_constant(tolua_S,"SDLK_WORLD_92",SDLK_WORLD_92);
  tolua_constant(tolua_S,"SDLK_WORLD_93",SDLK_WORLD_93);
  tolua_constant(tolua_S,"SDLK_WORLD_94",SDLK_WORLD_94);
  tolua_constant(tolua_S,"SDLK_WORLD_95",SDLK_WORLD_95);
  tolua_constant(tolua_S,"SDLK_KP0",SDLK_KP0);
  tolua_constant(tolua_S,"SDLK_KP1",SDLK_KP1);
  tolua_constant(tolua_S,"SDLK_KP2",SDLK_KP2);
  tolua_constant(tolua_S,"SDLK_KP3",SDLK_KP3);
  tolua_constant(tolua_S,"SDLK_KP4",SDLK_KP4);
  tolua_constant(tolua_S,"SDLK_KP5",SDLK_KP5);
  tolua_constant(tolua_S,"SDLK_KP6",SDLK_KP6);
  tolua_constant(tolua_S,"SDLK_KP7",SDLK_KP7);
  tolua_constant(tolua_S,"SDLK_KP8",SDLK_KP8);
  tolua_constant(tolua_S,"SDLK_KP9",SDLK_KP9);
  tolua_constant(tolua_S,"SDLK_KP_PERIOD",SDLK_KP_PERIOD);
  tolua_constant(tolua_S,"SDLK_KP_DIVIDE",SDLK_KP_DIVIDE);
  tolua_constant(tolua_S,"SDLK_KP_MULTIPLY",SDLK_KP_MULTIPLY);
  tolua_constant(tolua_S,"SDLK_KP_MINUS",SDLK_KP_MINUS);
  tolua_constant(tolua_S,"SDLK_KP_PLUS",SDLK_KP_PLUS);
  tolua_constant(tolua_S,"SDLK_KP_ENTER",SDLK_KP_ENTER);
  tolua_constant(tolua_S,"SDLK_KP_EQUALS",SDLK_KP_EQUALS);
  tolua_constant(tolua_S,"SDLK_UP",SDLK_UP);
  tolua_constant(tolua_S,"SDLK_DOWN",SDLK_DOWN);
  tolua_constant(tolua_S,"SDLK_RIGHT",SDLK_RIGHT);
  tolua_constant(tolua_S,"SDLK_LEFT",SDLK_LEFT);
  tolua_constant(tolua_S,"SDLK_INSERT",SDLK_INSERT);
  tolua_constant(tolua_S,"SDLK_HOME",SDLK_HOME);
  tolua_constant(tolua_S,"SDLK_END",SDLK_END);
  tolua_constant(tolua_S,"SDLK_PAGEUP",SDLK_PAGEUP);
  tolua_constant(tolua_S,"SDLK_PAGEDOWN",SDLK_PAGEDOWN);
  tolua_constant(tolua_S,"SDLK_F1",SDLK_F1);
  tolua_constant(tolua_S,"SDLK_F2",SDLK_F2);
  tolua_constant(tolua_S,"SDLK_F3",SDLK_F3);
  tolua_constant(tolua_S,"SDLK_F4",SDLK_F4);
  tolua_constant(tolua_S,"SDLK_F5",SDLK_F5);
  tolua_constant(tolua_S,"SDLK_F6",SDLK_F6);
  tolua_constant(tolua_S,"SDLK_F7",SDLK_F7);
  tolua_constant(tolua_S,"SDLK_F8",SDLK_F8);
  tolua_constant(tolua_S,"SDLK_F9",SDLK_F9);
  tolua_constant(tolua_S,"SDLK_F10",SDLK_F10);
  tolua_constant(tolua_S,"SDLK_F11",SDLK_F11);
  tolua_constant(tolua_S,"SDLK_F12",SDLK_F12);
  tolua_constant(tolua_S,"SDLK_F13",SDLK_F13);
  tolua_constant(tolua_S,"SDLK_F14",SDLK_F14);
  tolua_constant(tolua_S,"SDLK_F15",SDLK_F15);
  tolua_constant(tolua_S,"SDLK_NUMLOCK",SDLK_NUMLOCK);
  tolua_constant(tolua_S,"SDLK_CAPSLOCK",SDLK_CAPSLOCK);
  tolua_constant(tolua_S,"SDLK_SCROLLOCK",SDLK_SCROLLOCK);
  tolua_constant(tolua_S,"SDLK_RSHIFT",SDLK_RSHIFT);
  tolua_constant(tolua_S,"SDLK_LSHIFT",SDLK_LSHIFT);
  tolua_constant(tolua_S,"SDLK_RCTRL",SDLK_RCTRL);
  tolua_constant(tolua_S,"SDLK_LCTRL",SDLK_LCTRL);
  tolua_constant(tolua_S,"SDLK_RALT",SDLK_RALT);
  tolua_constant(tolua_S,"SDLK_LALT",SDLK_LALT);
  tolua_constant(tolua_S,"SDLK_RMETA",SDLK_RMETA);
  tolua_constant(tolua_S,"SDLK_LMETA",SDLK_LMETA);
  tolua_constant(tolua_S,"SDLK_LSUPER",SDLK_LSUPER);
  tolua_constant(tolua_S,"SDLK_RSUPER",SDLK_RSUPER);
  tolua_constant(tolua_S,"SDLK_MODE",SDLK_MODE);
  tolua_constant(tolua_S,"SDLK_COMPOSE",SDLK_COMPOSE);
  tolua_constant(tolua_S,"SDLK_HELP",SDLK_HELP);
  tolua_constant(tolua_S,"SDLK_PRINT",SDLK_PRINT);
  tolua_constant(tolua_S,"SDLK_SYSREQ",SDLK_SYSREQ);
  tolua_constant(tolua_S,"SDLK_BREAK",SDLK_BREAK);
  tolua_constant(tolua_S,"SDLK_MENU",SDLK_MENU);
  tolua_constant(tolua_S,"SDLK_POWER",SDLK_POWER);
  tolua_constant(tolua_S,"SDLK_EURO",SDLK_EURO);
  tolua_constant(tolua_S,"SDLK_UNDO",SDLK_UNDO);
  tolua_constant(tolua_S,"SDLK_LAST",SDLK_LAST);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_general (lua_State* tolua_S) {
 return tolua_general_open(tolua_S);
};
#endif

