/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

/* This file contains the prototypes for the toLua program to bind these
 * classes to Lua.
 */

$#include "core/math/vector.h"

$#include "world/world.h"
$#include "world/layer.h"
$#include "world/isodiamondlayer.h"
$#include "world/topdownlayer.h"
$#include "world/bound.h"
$#include "actionmap.h"
$#include "keymap.h"
$#include "creature.h"
$#include "particlesystem.h"
$#include "state.h"
$#include "sound.h"
$#include "idmanager.h"
$#include "modifierai.h"
$#include "keys.h"

enum LayerType { ETopDown, EIsoDiamond };

class Vector
{
	Vector();
	Vector(float x, float y);

	void normalize();
	float length();
	float dot(const Vector& v);
	void set (float x, float y);

	Vector operator+(const Vector& v);
	Vector operator-(const Vector& v);

	float x;
	float y;
};

class Color
{
	Color();
	Color(float r, float g, float b, float a=1.0f);

   // get/set
   float getRed() const;
   void  setRed(float red);

   float getGreen() const;
   void  setGreen(float green);

   float getBlue() const;
   void  setBlue(float blue);

   float getAlpha() const;
   void  setAlpha(float alpha);

 // operations
	void set(float red, float green, float blue, float alpha=1.0f);
};

class Particle
{
	void setColor (float r, float g, float b);
	Color color;
	int life;
	int state;
};

class Modifier
{
};

class ModifierAI : public Modifier
{
	ModifierAI(const std::string& function);
};

class State
{
   void duration(int time);
   int duration() const;

   void start(int s);
   int start() const;
};

class LocationState: public State
{
   LocationState();
   LocationState(const Vector& location);

   void location(const Vector& loc);
   Vector location() const;
};

class MoveState: public State
{
   MoveState();
   MoveState(int duration, const Vector& destination);

   void destination(const Vector& dest);
   Vector destination() const;
};

class RotateState: public State
{
   RotateState();
   RotateState(int duration, float rotation);

   void rotation(float angle);
   float rotation() const;
};

class SceneObject
{
   void add(SceneObject* child);
   
   unsigned int getId();

   SceneObject* find(const unsigned int& id);

   void setName(const std::string& name);
   const std::string& getName();
};

class Object: public SceneObject
{
	Object();

   bool create(const char* filename);

	void flip ();
	bool direction ();
   void rotate(float deg);

   void setPosition(const Vector& pos);
   void setVelocity(const Vector& vel);
   void setRotation(const float deg);

	Vector getPosition ();
   Vector getVelocity();
   float getRotation();

   Body& getBody();

   int getAnimation() const;
	void setAnimation (int anim);

   void addState(State* state);
   void addModifier(Modifier* pmodifier);
};

class Creature: public Object
{
	Creature();

	bool create(const char* filename);
	void moveto (Creature* t);
};

class ParticleSystem: public Object
{
   ParticleSystem();

   void setEmitRate(int rate);
   void setEmitCount(int count);
};

class Layer
{
   bool create(const std::string& name, int width, int height, const std::string& effect);
   
   const std::string& getName() const;
   int                getTile(int x, int y);
   void               setTile(int x, int y, int tile);
   Vector             getDimensions();
   Vector             getScroll();
   Vector             getScrollSpeed() const;

   Vector   screenToLayer(const Vector& v);
   void     enableTileAnimation(bool enable);
   void     resize(int newwidth, int newheight);
   void     setScrollSpeed(float newx, float newy);
};

class TopDownLayer : public Layer
{
   TopDownLayer();
};

class IsoDiamondLayer : public Layer
{
   IsoDiamondLayer();
};

class Bound
{
   Bound();

   void  move(const Vector& offset);
   int   findPoint(const Vector& point);
   void  movePoint(int point, const Vector& offset);
};

class World: public SceneObject
{
   World();
   ~World();

   enum FollowMode { FollowObject=0, FollowMouse=1, NoFollow=2 };

   Layer* createLayer();
   int    addLayer(Layer* player);
   void   moveLayer(Layer* player, int offset);
   int    getLayerCount() const;
   Layer* getLayer(int index);
   void   setLayerType(int type);

   Bound& addBound(const Vector& p1, const Vector& p2);
   void   removeBound(Bound& bound);

   bool create(const char* filename);
   void createEmpty(const std::string& name);
   void destroy();
   void save();

   void loadObjects(const char* filename);

   void setAutoFollow(bool enabled=true);
   void setScrollMode(int fm);
   void setFollowObject(Object* obj);
   void setFollowBorderWidth(int width);
   void setFollowBorders(int left, int right, int top, int bottom);
   void setObjectLayer(int index);

   Simulator& getSimulator();
};

class SceneGraph
{
   void setWorld(World* world);
   void setControler(Object* controler);

   World* getWorld();
   Object* getControler();

   void removeAll();

   SceneObject* find(const unsigned int& id);
};

class ActionMap
{
   ActionMap();

   void bind(int action, const char* function);
};

class KeyMap
{
   KeyMap();

   void bind(int key, int action);
};

class Sound
{
   void play (bool loop = false);
   void stop ();
};

class SoundManager
{
   Sound* load (const char* file);
};

namespace Keys
{
   enum {
      eKey_tab,
      eKey_space,
      eKey_backspace,
      eKey_lshift,
      eKey_rshift,
      eKey_lalt,
      eKey_ralt,
      eKey_lcontrol,
      eKey_rcontrol,
      eKey_escape,
      eKey_break,
      eKey_backquote,      // `
      eKey_a,
      eKey_b,
      eKey_c,
      eKey_d,
      eKey_e,
      eKey_f,
      eKey_g,
      eKey_h,
      eKey_i,
      eKey_j,
      eKey_k,
      eKey_l,
      eKey_m,
      eKey_n,
      eKey_o,
      eKey_p,
      eKey_q,
      eKey_r,
      eKey_s,
      eKey_t,
      eKey_u,
      eKey_v,
      eKey_w,
      eKey_x,
      eKey_y,
      eKey_z,
      eKey_1,
      eKey_2,
      eKey_3,
      eKey_4,
      eKey_5,
      eKey_6,
      eKey_7,
      eKey_8,
      eKey_9,
      eKey_colon,
      eKey_semicolon,
      eKey_minus,
      eKey_plus,
      eKey_less,
      eKey_greater,
      eKey_equal,
      eKey_leftbracket,    // [
      eKey_rightbracket,   // ]
      eKey_backslash,      // /
      eKey_caret,          // |
      eKey_underscore,     // _
      eKey_enter,
      eKey_home,
      eKey_pageUp,
      eKey_pageDown,
      eKey_end,
      eKey_insert,
      eKey_delete,
      eKey_up,
      eKey_down,
      eKey_left,
      eKey_right,
      eKey_numlock,
      eKey_capslock,
      eKey_scrollock,
      eKey_F1,
      eKey_F2,
      eKey_F3,
      eKey_F4,
      eKey_F5,
      eKey_F6,
      eKey_F7,
      eKey_F8,
      eKey_F9,
      eKey_F10,
      eKey_F11,
      eKey_F12,
      eKey_kp_dot,
      eKey_kp_min,
      eKey_kp_plus,
      eKey_kp_multiply,
      eKey_kp_divide,
      eKey_kp_equal,
      eKey_kp_insert,
      eKey_kp_delete,
      eKey_kp_enter,
      eKey_kp_1,
      eKey_kp_2,
      eKey_kp_3,
      eKey_kp_4,
      eKey_kp_5,
      eKey_kp_6,
      eKey_kp_7,
      eKey_kp_8,
      eKey_kp_9,
   };
};
