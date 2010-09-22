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
#include "defines.h"

/// \fn Object::flip()
/// \brief Flip the direction the object is looking at.
INLINE void Object::flip ()
{
   dir = !dir;

   setDirty(ePositionDirty);
}

/// \fn Object::direction()
/// \brief Returns the direction the object is looking at.
/// \retval true the object is facing to the right
/// \retval false the object is facing to the left
INLINE bool Object::direction() const
{
   return dir;
}

/// \fn Object::rotate(float degree)
/// \brief Rotate the object by degree degrees.
INLINE void Object::rotate(float degree)
{
   setRotation(angle+degree); 
}

/// \fn Object::setVelocity (const Vector& v)
/// \brief Set the velocity of the object. The velocity should be measured in seconds,
/// as the engine will interpolate it between the frames displayed.
/// \param v the new velocity in distance/second
INLINE void Object::setVelocity (const Vector& v)
{
	mVel = v;
   setDirty(ePositionDirty);
}

/// \fn void setRotation(const float deg)
/// \brief Sets the new degree of rotation of this object.
/// \param deg the new rotation degree
INLINE void Object::setRotation(const float deg) 
{
   angle = deg;
   setDirty(ePositionDirty);

   if (angle<0) 
      angle+=360;
   else if (angle>=360) 
      angle -= 360;
}

/// \fn Object::setVisible (bool vis = true)
/// \brief Set the visibility status of this object.
/// \param vis the visibility flag for this object
INLINE void Object::setVisible(bool vis)
{
	visible = vis;
}

/// \fn Object::isVisible ()
/// \brief Returns the current visibility status of this object.
/// \retval true the object is visible
/// \retval false the object is invisible
INLINE bool Object::isVisible() const
{
	return visible;
}

/// \fn Object::isStatic() const
/// \brief Check if this is a static object.
INLINE bool Object::isStatic() const
{
   return mStatic;
}

/// \fn Object::getRadius() const
/// \brief Returns the radius of the object.
/// \returns the radius of the object
INLINE float Object::getRadius() const
{
   return radius; 
}

/// \fn Object::getRotation() const
/// \brief Returns the rotation in degrees of the object.
INLINE float Object::getRotation() const
{
   return angle;
}

/// \fn Object::getVelocity ()
/// \brief Returns the current velocity of the object
/// \returns current velocity of the object
INLINE Vector Object::getVelocity() const
{
	return mVel;
}

/// \fn Object::getSize()
/// \brief Returns the size of this object.
INLINE Vector Object::getSize() const
{
   return Vector(static_cast<float>(width), static_cast<float>(height)); 
}

INLINE const Texture& Object::getTexture() const
{
   return *texture;
}

INLINE Animator& Object::getAnimator()
{
   return *mpAnimator;
}

INLINE bool Object::hasBody() const
{
   return mpBody != NULL;
}

INLINE Body& Object::getBody()
{
   return *mpBody;
}