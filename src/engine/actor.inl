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
#include "core/defines.h"

/// \fn Actor::flip()
/// \brief Flip the direction the object is looking at.
INLINE void Actor::flip ()
{
   dir = !dir;

   setDirty(ePositionDirty);
}

/// \fn Actor::direction()
/// \brief Returns the direction the object is looking at.
/// \retval true the object is facing to the right
/// \retval false the object is facing to the left
INLINE bool Actor::direction() const
{
   return dir;
}

/// \fn Actor::rotate(float degree)
/// \brief Rotate the object by degree degrees.
INLINE void Actor::rotate(float degree)
{
   setRotation(angle+degree); 
}

/// \fn Actor::setVelocity (const Vector& v)
/// \brief Set the velocity of the object. The velocity should be measured in seconds,
/// as the engine will interpolate it between the frames displayed.
/// \param v the new velocity in distance/second
INLINE void Actor::setVelocity (const Vector& v)
{
	mVel = v;
   setDirty(ePositionDirty);
}

/// \fn void setRotation(const float deg)
/// \brief Sets the new degree of rotation of this object.
/// \param deg the new rotation degree
INLINE void Actor::setRotation(const float deg) 
{
   angle = deg;
   setDirty(ePositionDirty);

   if (angle<0) 
      angle+=360;
   else if (angle>=360) 
      angle -= 360;
}

/// \fn Actor::setVisible (bool vis = true)
/// \brief Set the visibility status of this object.
/// \param vis the visibility flag for this object
INLINE void Actor::setVisible(bool vis)
{
	visible = vis;
}

/// \fn Actor::isVisible ()
/// \brief Returns the current visibility status of this object.
/// \retval true the object is visible
/// \retval false the object is invisible
INLINE bool Actor::isVisible() const
{
	return visible;
}

/// \fn Actor::isStatic() const
/// \brief Check if this is a static object.
INLINE bool Actor::isStatic() const
{
   return mStatic;
}

/// \fn Actor::setStatic() const
/// \brief Set the static object flag.
INLINE void Actor::setStatic(bool isstatic)
{
   mStatic = isstatic;
}

/// \fn Actor::getRadius() const
/// \brief Returns the radius of the object.
/// \returns the radius of the object
INLINE float Actor::getRadius() const
{
   return radius; 
}

/// \fn Actor::getRotation() const
/// \brief Returns the rotation in degrees of the object.
INLINE float Actor::getRotation() const
{
   return angle;
}

/// \fn Actor::getVelocity ()
/// \brief Returns the current velocity of the object
/// \returns current velocity of the object
INLINE const Vector& Actor::getVelocity() const
{
	return mVel;
}

/// \fn Actor::getSize()
/// \brief Returns the size of this object.
INLINE Vector Actor::getSize() const
{
   return Vector(static_cast<float>(mWidth), static_cast<float>(mHeight)); 
}

INLINE const Texture& Actor::getTexture() const
{
   return *texture;
}

INLINE Animator& Actor::getAnimator()
{
   return *mpAnimator;
}

INLINE bool Actor::hasBody() const
{
   return mpBody != NULL;
}

INLINE Body& Actor::getBody()
{
   return *mpBody;
}

INLINE Controller& Actor::getController()
{
   return *mpController;
}

const std::string& Actor::getType() const
{
   return mType;
}

void Actor::setType(const std::string& type)
{
   mType = type;
}

INLINE void Actor::setTexture(TexturePtr texture)
{
   this->texture = texture;
}

INLINE void Actor::setAnimator(Animator* panimator)
{
   mpAnimator = panimator;
}

INLINE void Actor::setBody(Body& body)
{
   mpBody = &body;
}