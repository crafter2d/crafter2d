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

#include "physics/body.h"
#include "physics/simulator.h"

#include "world/world.h"

#include "animator.h"
#include "scenegraph.h"
#include "log.h"
#include "state.h"
#include "nodevisitor.h"
#include "texturecoordinate.h"

IMPLEMENT_REPLICATABLE(ObjectId, Object, SceneObject)

Object::Object():
   SceneObject(),
   texture(),
   mpBody(NULL),
   mpAnimator(NULL),
	width(0),
   height(0),
   halfX(.0f),
   halfY(.0f),
   angle(.0f),
   dir(true),
   radius(.0f),
	visible(true),
   mStatic(false)
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
	Log& log = Log::getInstance();

	// try to find the object in the file
	TiXmlElement* object = (TiXmlElement*)doc.FirstChild ("object");
	if (object == NULL)
   {
      log.error("Object.load: Invalid XML file format, object expected.\n");
		return false;
	}

	// find the name and dimensions of the object
   if ( !hasName() )
	   setName(object->Attribute("name"));

	if ( object->QueryIntAttribute ("width", &width) != TIXML_SUCCESS ||
		  object->QueryIntAttribute ("height", &height) != TIXML_SUCCESS )
   {
      log.error("Object.load: object needs to have dimensions.\n");
		return false;
	}

   // see whether or not the object is static
   int temp = 0;
   if ( object->QueryIntAttribute("static", &temp) == TIXML_SUCCESS )
      mStatic = (temp == 1);

	// determine radius of object
	radius = height * 0.5f;

	// determine upper left corner
	halfX = width * 0.5f;
	halfY = height * 0.5f;

	// load texture data
	TiXmlElement* tex = (TiXmlElement*)object->FirstChild ("texture");
	if ( tex == NULL )
   {
		log.error("Object.load: object has no texture");
		return false;
	}
	else
   {
		TiXmlText* value = (TiXmlText*)tex->FirstChild();
      texture = ResourceManager::getInstance().loadTexture(value->Value());
      if ( !texture.valid() )
      {
         log.error("Object.load: can not load %s", value->Value());
         return false;
      }
	}

   // load animation stuff
   mpAnimator = Animator::construct(object, *this);

   // load simulation info
   if ( Body::hasInfo(*object) )
   {
      World& world = *(getSceneGraph().getWorld());

      mpBody = &world.getSimulator().createBody(*this);
      mpBody->load(*object);
   }

	return true;
}

void Object::destroy()
{
   SceneObject::destroy();

   delete mpBody;
   mpBody = NULL;
}

// - Modifier interface

void Object::addModifier(Modifier* pmodifier)
{
   ASSERT_PTR(pmodifier)
   mModifiers.push_back(pmodifier);
}

// - Updating

void Object::doUpdate(float delta)
{
   for ( Modifiers::size_type index = 0; index < mModifiers.size(); index++ )
   {
      Modifier* pmodifier = mModifiers[index];
      pmodifier->update(*this, delta);
   }
}

void Object::doUpdateClient(float delta)
{
   // perform client side predictions
   if ( mpAnimator != NULL )
      mpAnimator->animate(delta);  
}

/*!
    \fn Object::draw()
	 \brief Draws the object on the screen.
 */
void Object::doDraw()
{
	texture->enable();

   TextureCoordinate texcoord = mpAnimator->getTextureCoordinate();
   if ( !dir )
      texcoord.flipHorizontal();

   Vector tex;

   glColor3f (1,1,1);
   glPushMatrix();

   glRotatef(angle, 0,0,1);
	glBegin (GL_QUADS);
      tex = texcoord.getTopLeft();
		glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (-halfX,-halfY);

      tex = texcoord.getBottomLeft();
      glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (-halfX,halfY);

      tex = texcoord.getBottomRight();
      glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (halfX,halfY);

      tex = texcoord.getTopRight();
      glMultiTexCoord2f (GL_TEXTURE0_ARB, tex.x, tex.y);
		glVertex2f (halfX,-halfY);
	glEnd();

   texture->disable();

   glPopMatrix();
}

/*!
    \fn Object::move(float delta)
	 \brief If the difference between this call and the previous call to this function
	 is larger than the movement rate, the position of the object is updated using the
	 objects velocity.
	 \param tick the time tick of this frame (in milliseconds)
 */
void Object::move(float delta)
{
   ASSERT(!isReplica())

   /*
   if ( mVel != Vector::zero() ) 
   {
      SceneGraph& graph = getSceneGraph();
      graph.getWorld()->collide(*this, mPos);

      pos += vel;
      
      setDirty(ePositionDirty);
   }
   */
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

/// \fn Object::getPosition () const
/// \brief Returns the current position of the object
/// \returns current position of object
const Vector& Object::getPosition() const
{
   return mPos;
}

/// \fn Object::setPosition(const Vector& p)
/// \brief Set the position of the object in world coordinates.
/// \param p the new position of the object
void Object::setPosition(const Vector& p)
{
   if ( isReplica() )
      mPos = p;
   else
   {
      if ( mpBody != NULL )
         mpBody->setPosition(p);
      else
         mPos = p;

      setDirty(ePositionDirty);
   }
}

int Object::getAnimation() const
{
   return mpAnimator != NULL ? mpAnimator->getAnimation() : 0;
}

void Object::setAnimation(int anim)
{
   if ( mpAnimator != NULL )
   {
      mpAnimator->setAnimation(anim);

      setDirty(eAnimationDirty);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Visitor interface
//////////////////////////////////////////////////////////////////////////

void Object::accept(NodeVisitor& nv)
{
   nv.visitObject(this);
}

// simulator interface

void Object::updateState()
{
   ASSERT_PTR(mpBody);

   mPos  = mpBody->getPosition();
   angle = mpBody->getAngle();

   setDirty(ePositionDirty);
}

//////////////////////////////////////////////////////////////////////////
// - Packing & unpacking
//////////////////////////////////////////////////////////////////////////

void Object::pack(BitStream& stream) const
{
   SceneObject::pack(stream);
   stream << dirtyFlag;

   if ( IS_SET(dirtyFlag, ePositionDirty) )
      stream << getPosition() << mVel << angle << dir;

   if ( IS_SET(dirtyFlag, eAnimationDirty) )
      stream << mpAnimator->getAnimation();
}

void Object::unpack(BitStream& stream)
{
   int flag;
   SceneObject::unpack(stream);
   stream >> flag;

   if ( IS_SET(flag, ePositionDirty) )
   {
      Vector pos;
      stream >> pos >> mVel >> angle >> dir;

      setPosition(pos);
   }

   if ( IS_SET(flag, eAnimationDirty) )
   {
      int anim;

      stream >> anim;

      if ( mpAnimator != NULL )
         mpAnimator->setAnimation(anim);
   }
}
