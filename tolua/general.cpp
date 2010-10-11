/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* This file contains the prototypes for the toLua program to bind these
 * classes to Lua.
 */

$#include "math/vector.h"
$#include "world/world.h"
$#include "world/layer.h"
$#include "world/isodiamondlayer.h"
$#include "world/topdownlayer.h"
$#include "world/bound.h"
$#include "actionmap.h"
$#include "creature.h"
$#include "particlesystem.h"
$#include "state.h"
$#include "script.h"
$#include "scriptmanager.h"
$#include "sound.h"
$#include "idmanager.h"
$#include "modifierai.h"

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

   void bind(int key, const char* function);
};

class KeyMap
{
   KeyMap();

   void bind(int key, int action, bool local=false);
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

enum {
   /* the actions to be send to the player */
   moveLeft,
   moveRight,
   moveUp,
   moveDown,

   jump,

   swapConsole,
   quit
};

enum {
	/* The keyboard syms have been cleverly chosen to map to ASCII */
   /* Copied from the SDL library */
	SDLK_UNKNOWN		= 0,
	SDLK_FIRST		= 0,
	SDLK_BACKSPACE		= 8,
	SDLK_TAB		= 9,
	SDLK_CLEAR		= 12,
	SDLK_RETURN		= 13,
	SDLK_PAUSE		= 19,
	SDLK_ESCAPE		= 27,
	SDLK_SPACE		= 32,
	SDLK_EXCLAIM		= 33,
	SDLK_QUOTEDBL		= 34,
	SDLK_HASH		= 35,
	SDLK_DOLLAR		= 36,
	SDLK_AMPERSAND		= 38,
	SDLK_QUOTE		= 39,
	SDLK_LEFTPAREN		= 40,
	SDLK_RIGHTPAREN		= 41,
	SDLK_ASTERISK		= 42,
	SDLK_PLUS		= 43,
	SDLK_COMMA		= 44,
	SDLK_MINUS		= 45,
	SDLK_PERIOD		= 46,
	SDLK_SLASH		= 47,
	SDLK_0			= 48,
	SDLK_1			= 49,
	SDLK_2			= 50,
	SDLK_3			= 51,
	SDLK_4			= 52,
	SDLK_5			= 53,
	SDLK_6			= 54,
	SDLK_7			= 55,
	SDLK_8			= 56,
	SDLK_9			= 57,
	SDLK_COLON		= 58,
	SDLK_SEMICOLON		= 59,
	SDLK_LESS		= 60,
	SDLK_EQUALS		= 61,
	SDLK_GREATER		= 62,
	SDLK_QUESTION		= 63,
	SDLK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	SDLK_LEFTBRACKET	= 91,
	SDLK_BACKSLASH		= 92,
	SDLK_RIGHTBRACKET	= 93,
	SDLK_CARET		= 94,
	SDLK_UNDERSCORE		= 95,
	SDLK_BACKQUOTE		= 96,
	SDLK_a			= 97,
	SDLK_b			= 98,
	SDLK_c			= 99,
	SDLK_d			= 100,
	SDLK_e			= 101,
	SDLK_f			= 102,
	SDLK_g			= 103,
	SDLK_h			= 104,
	SDLK_i			= 105,
	SDLK_j			= 106,
	SDLK_k			= 107,
	SDLK_l			= 108,
	SDLK_m			= 109,
	SDLK_n			= 110,
	SDLK_o			= 111,
	SDLK_p			= 112,
	SDLK_q			= 113,
	SDLK_r			= 114,
	SDLK_s			= 115,
	SDLK_t			= 116,
	SDLK_u			= 117,
	SDLK_v			= 118,
	SDLK_w			= 119,
	SDLK_x			= 120,
	SDLK_y			= 121,
	SDLK_z			= 122,
	SDLK_DELETE		= 127,
	/* End of ASCII mapped SDLKsyms */

	/* International SDLKboard syms */
	SDLK_WORLD_0		= 160,		/* 0xA0 */
	SDLK_WORLD_1		= 161,
	SDLK_WORLD_2		= 162,
	SDLK_WORLD_3		= 163,
	SDLK_WORLD_4		= 164,
	SDLK_WORLD_5		= 165,
	SDLK_WORLD_6		= 166,
	SDLK_WORLD_7		= 167,
	SDLK_WORLD_8		= 168,
	SDLK_WORLD_9		= 169,
	SDLK_WORLD_10		= 170,
	SDLK_WORLD_11		= 171,
	SDLK_WORLD_12		= 172,
	SDLK_WORLD_13		= 173,
	SDLK_WORLD_14		= 174,
	SDLK_WORLD_15		= 175,
	SDLK_WORLD_16		= 176,
	SDLK_WORLD_17		= 177,
	SDLK_WORLD_18		= 178,
	SDLK_WORLD_19		= 179,
	SDLK_WORLD_20		= 180,
	SDLK_WORLD_21		= 181,
	SDLK_WORLD_22		= 182,
	SDLK_WORLD_23		= 183,
	SDLK_WORLD_24		= 184,
	SDLK_WORLD_25		= 185,
	SDLK_WORLD_26		= 186,
	SDLK_WORLD_27		= 187,
	SDLK_WORLD_28		= 188,
	SDLK_WORLD_29		= 189,
	SDLK_WORLD_30		= 190,
	SDLK_WORLD_31		= 191,
	SDLK_WORLD_32		= 192,
	SDLK_WORLD_33		= 193,
	SDLK_WORLD_34		= 194,
	SDLK_WORLD_35		= 195,
	SDLK_WORLD_36		= 196,
	SDLK_WORLD_37		= 197,
	SDLK_WORLD_38		= 198,
	SDLK_WORLD_39		= 199,
	SDLK_WORLD_40		= 200,
	SDLK_WORLD_41		= 201,
	SDLK_WORLD_42		= 202,
	SDLK_WORLD_43		= 203,
	SDLK_WORLD_44		= 204,
	SDLK_WORLD_45		= 205,
	SDLK_WORLD_46		= 206,
	SDLK_WORLD_47		= 207,
	SDLK_WORLD_48		= 208,
	SDLK_WORLD_49		= 209,
	SDLK_WORLD_50		= 210,
	SDLK_WORLD_51		= 211,
	SDLK_WORLD_52		= 212,
	SDLK_WORLD_53		= 213,
	SDLK_WORLD_54		= 214,
	SDLK_WORLD_55		= 215,
	SDLK_WORLD_56		= 216,
	SDLK_WORLD_57		= 217,
	SDLK_WORLD_58		= 218,
	SDLK_WORLD_59		= 219,
	SDLK_WORLD_60		= 220,
	SDLK_WORLD_61		= 221,
	SDLK_WORLD_62		= 222,
	SDLK_WORLD_63		= 223,
	SDLK_WORLD_64		= 224,
	SDLK_WORLD_65		= 225,
	SDLK_WORLD_66		= 226,
	SDLK_WORLD_67		= 227,
	SDLK_WORLD_68		= 228,
	SDLK_WORLD_69		= 229,
	SDLK_WORLD_70		= 230,
	SDLK_WORLD_71		= 231,
	SDLK_WORLD_72		= 232,
	SDLK_WORLD_73		= 233,
	SDLK_WORLD_74		= 234,
	SDLK_WORLD_75		= 235,
	SDLK_WORLD_76		= 236,
	SDLK_WORLD_77		= 237,
	SDLK_WORLD_78		= 238,
	SDLK_WORLD_79		= 239,
	SDLK_WORLD_80		= 240,
	SDLK_WORLD_81		= 241,
	SDLK_WORLD_82		= 242,
	SDLK_WORLD_83		= 243,
	SDLK_WORLD_84		= 244,
	SDLK_WORLD_85		= 245,
	SDLK_WORLD_86		= 246,
	SDLK_WORLD_87		= 247,
	SDLK_WORLD_88		= 248,
	SDLK_WORLD_89		= 249,
	SDLK_WORLD_90		= 250,
	SDLK_WORLD_91		= 251,
	SDLK_WORLD_92		= 252,
	SDLK_WORLD_93		= 253,
	SDLK_WORLD_94		= 254,
	SDLK_WORLD_95		= 255,		/* 0xFF */

	/* Numeric SDLKpad */
	SDLK_KP0		= 256,
	SDLK_KP1		= 257,
	SDLK_KP2		= 258,
	SDLK_KP3		= 259,
	SDLK_KP4		= 260,
	SDLK_KP5		= 261,
	SDLK_KP6		= 262,
	SDLK_KP7		= 263,
	SDLK_KP8		= 264,
	SDLK_KP9		= 265,
	SDLK_KP_PERIOD		= 266,
	SDLK_KP_DIVIDE		= 267,
	SDLK_KP_MULTIPLY	= 268,
	SDLK_KP_MINUS		= 269,
	SDLK_KP_PLUS		= 270,
	SDLK_KP_ENTER		= 271,
	SDLK_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	SDLK_UP			= 273,
	SDLK_DOWN		= 274,
	SDLK_RIGHT		= 275,
	SDLK_LEFT		= 276,
	SDLK_INSERT		= 277,
	SDLK_HOME		= 278,
	SDLK_END		= 279,
	SDLK_PAGEUP		= 280,
	SDLK_PAGEDOWN		= 281,

	/* Function SDLKs */
	SDLK_F1			= 282,
	SDLK_F2			= 283,
	SDLK_F3			= 284,
	SDLK_F4			= 285,
	SDLK_F5			= 286,
	SDLK_F6			= 287,
	SDLK_F7			= 288,
	SDLK_F8			= 289,
	SDLK_F9			= 290,
	SDLK_F10		= 291,
	SDLK_F11		= 292,
	SDLK_F12		= 293,
	SDLK_F13		= 294,
	SDLK_F14		= 295,
	SDLK_F15		= 296,

	/* SDLK state modifier SDLKs */
	SDLK_NUMLOCK		= 300,
	SDLK_CAPSLOCK		= 301,
	SDLK_SCROLLOCK		= 302,
	SDLK_RSHIFT		= 303,
	SDLK_LSHIFT		= 304,
	SDLK_RCTRL		= 305,
	SDLK_LCTRL		= 306,
	SDLK_RALT		= 307,
	SDLK_LALT		= 308,
	SDLK_RMETA		= 309,
	SDLK_LMETA		= 310,
	SDLK_LSUPER		= 311,		/* Left "Windows" SDLK */
	SDLK_RSUPER		= 312,		/* Right "Windows" SDLK */
	SDLK_MODE		= 313,		/* "Alt Gr" SDLK */
	SDLK_COMPOSE		= 314,		/* Multi-SDLK compose SDLK */

	/* Miscellaneous function SDLKs */
	SDLK_HELP		= 315,
	SDLK_PRINT		= 316,
	SDLK_SYSREQ		= 317,
	SDLK_BREAK		= 318,
	SDLK_MENU		= 319,
	SDLK_POWER		= 320,		/* Power Macintosh power SDLK */
	SDLK_EURO		= 321,		/* Some european SDLKboards */
	SDLK_UNDO		= 322,		/* Atari SDLKboard has Undo */

	/* Add any other SDLKs here */

	SDLK_LAST
}
