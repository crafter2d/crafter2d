/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#include "object.h"
#ifndef JENGINE_INLINE
#  include "object.inl"
#endif

#include <GL/GLee.h>
#include <GL/glu.h>
#include <tinyxml.h>

#include "world/world.h"

#include "scenegraph.h"
#include "game.h"
#include "console.h"
#include "state.h"
#include "nodevisitor.h"

IMPLEMENT_REPLICATABLE(ObjectId, Object, SceneObject)

Object::Object():
   SceneObject(),
   texture(),
	width(0),
   height(0),
   halfX(.0f),
   halfY(.0f),
   angle(.0f),
   dir(true),
   radius(.0f),
	moveSpeed(0),
	moveLast(0),
	visible(true),
   mMass(false)
{
}

Object::~Object()
{
}

/*!
    \fn Object::load (TiXmlDocument& doc)
	 \brief Loads the object from the XML document (using the TinyXML library).
	 \param doc an open XML document to load from
	 \retval true no errors occured during load
	 \retval false invalid file format, consult the log file for the missing part
 */
bool Object::load (TiXmlDocument& doc)
{
	TiXmlElement *object = NULL, *tex = NULL;
   Console& console = Console::getInstance();
   int temp = 0;

	// try to find the object in the file
	object = (TiXmlElement*)doc.FirstChild ("object");
	if (object == NULL)
   {
      console.print("Object.load: Invalid XML file format, object expected.\n");
		return false;
	}

	// find the name and dimensions of the object
	name = object->Attribute("name");
	if (object->QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		object->QueryIntAttribute ("height", &height) != TIXML_SUCCESS)
   {
      console.print("Object.load: object needs to have dimensions.\n");
		return false;
	}

   // check the mass of the object
   if ( object->QueryIntAttribute("mass", &temp) == TIXML_SUCCESS )
      setMass(temp != 0);

   // see whether or not the object is static
   if ( object->QueryIntAttribute("static", &temp) == TIXML_SUCCESS )
      statik = (temp == 1);

	// determine radius of object
	radius = height * 0.5f;

	// determine upper left corner
	halfX = width * 0.5f;
	halfY = height * 0.5f;

	// load texture data
	tex = (TiXmlElement*)object->FirstChild ("texture");
	if ( tex == NULL )
   {
		console.print("Object.load: object has no texture");
		return false;
	}
	else
   {
		TiXmlText* value = (TiXmlText*)tex->FirstChild();
      texture = ResourceManager::getInstance().loadTexture(value->Value());
      if ( !texture.valid() )
      {
         console.printf("Object.load: can not load %s", value->Value());
         return false;
      }
	}
	return true;
}

void Object::update(Uint32 tick)
{
   dirty = false;
   dirtyFlag = 0;

   if ( states.size() > 0 )
   {
      if ( states.front()->update(tick) )
         states.pop();
   }
   else
   {
      move(tick);
   }
}

/*!
    \fn Object::draw()
	 \brief Draws the object on the screen.
 */
void Object::draw()
{
	glPushMatrix ();
	glTranslatef (-halfX, -halfY, 0);

	texture->enable();

	if (dir)
   {
		glBegin (GL_QUADS);
			glMultiTexCoord2f (GL_TEXTURE0_ARB, 0, 0);
			glVertex2f (pos.x,pos.y);

         glMultiTexCoord2f (GL_TEXTURE0_ARB, 0, texture->getSourceHeight());
			glVertex2f (pos.x,pos.y+height);

         glMultiTexCoord2f (GL_TEXTURE0_ARB, texture->getSourceWidth(), texture->getSourceHeight());
			glVertex2f (pos.x+width,pos.y+height);

         glMultiTexCoord2f (GL_TEXTURE0_ARB, texture->getSourceWidth(), 0);
			glVertex2f (pos.x+width,pos.y);
		glEnd();
	}
	else
   {
		glBegin (GL_QUADS);
			glMultiTexCoord2f (GL_TEXTURE0_ARB, texture->getSourceWidth(), 0);
			glVertex2f (pos.x,pos.y);

			glMultiTexCoord2f (GL_TEXTURE0_ARB, texture->getSourceWidth(), texture->getSourceHeight());
			glVertex2f (pos.x,pos.y+height);

			glMultiTexCoord2f (GL_TEXTURE0_ARB, 0, texture->getSourceHeight());
			glVertex2f (pos.x+width,pos.y+height);

			glMultiTexCoord2f (GL_TEXTURE0_ARB, 0, 0);
			glVertex2f (pos.x+width,pos.y);
		glEnd();
	}

   texture->disable();

	glPopMatrix ();
}

/*!
    \fn Object::move (Uint32 tick)
	 \brief If the difference between this call and the previous call to this function
	 is larger than the movement rate, the position of the object is updated using the
	 objects velocity.
	 \param tick the time tick of this frame (in milliseconds)
 */
void Object::move (Uint32 tick)
{
	if (tick - moveLast >= moveSpeed)
   {
      applyGravity();

      if (vel.x != 0 || vel.y != 0) 
      {
         SceneGraph& graph = Game::getInstance().getServer().getSceneGraph();
         graph.getWorld()->collide(*this);

         pos += vel;

         if ( !isReplica() )
         {
		      moveLast = tick;
            dirtyFlag |= 4;
            dirty = true;
         }
      }
	}
}

/*!
    \fn Object::applyGravity (Uint32 tick)
	 \brief At the same rate as the movement the engine can add gravity to your object. This
	 means that the velocity becomes larger downwards.
	 \param tick the time tick of this frame (in milliseconds)
 */
void Object::applyGravity()
{
	if ( hasMass() )
   {
		vel.y += 1;
	}
}

/*!
    \fn Object::clone ()
	 \brief Currently not implemented yet!
	 \returns always NULL
 */
Object* Object::clone ()
{
	return NULL;
}

/// \fn Object:addState(State* state)
/// \brief Adds a new state to the state list.
void Object::addState(State* state)
{
   state->object(this);
   states.push(state);
}

//////////////////////////////////////////////////////////////////////////
// - Visitor interface
//////////////////////////////////////////////////////////////////////////

void Object::accept(NodeVisitor& nv)
{
   nv.visitObject(this);
}

//////////////////////////////////////////////////////////////////////////
// - Packing & unpacking
//////////////////////////////////////////////////////////////////////////

void Object::pack(BitStream& stream) const
{
   SceneObject::pack(stream);
   if (dirtyFlag & 4)
      stream << true << pos << vel << angle << dir;
   else
      stream << false;
}

void Object::unpack(BitStream& stream)
{
   char filen[256];
   bool flag;
   SceneObject::unpack(stream);
   stream >> flag;
   if (flag)
      stream >> pos >> vel >> angle >> dir;

   xmlfile = filen;
}
