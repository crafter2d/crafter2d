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

INLINE Controller& Actor::getController()
{
   return *mpController;
}
