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

#include "layer.h"

/// \fn World::setAutoFollow(bool enabled)
/// \brief Enable or disable auto follow mode of the camera.
INLINE void World::setAutoFollow(bool enabled)
{
   autoFollow = enabled;
}

INLINE Entity* World::getFollowObject()
{
   return followObject;
}

/// \fn World::setFollowObject(Entity& obj)
/// \brief Set the object which should be followed when in auto modus.
INLINE void World::setFollowObject(Entity& obj)
{
   followObject = &obj;
}

/// \fn World::setScrollMode(int fm)
/// \brief Set the scrolling mode of the engine. See FollowMode for more information.
INLINE void World::setScrollMode(int fm)
{
   followMode = static_cast<FollowMode>(fm);
}

/// \fn World::setFollowBorderWidth(int width)
/// \brief Set the border width of the sides of the window, used for mouse follow.
INLINE void World::setFollowBorderWidth(int width)
{
   followBorderWidth = width;
}

/// \fn World::setFollowBorders(int left, int right, int top, int bottom)
/// \brief Set the borders used in auto object follow modus.
INLINE void World::setFollowBorders(int left, int right, int top, int bottom)
{
   leftBorder = left;
   rightBorder = right;
   topBorder = top;
   bottomBorder = bottom;
}

INLINE const String& World::getName() const
{
   return mName;
}
   
INLINE void World::setName(const String& name)
{
   mName = name;
}

INLINE const String& World::getFilename() const
{
   return mFilename;
}

INLINE void World::setFilename(const String& filename)
{
   mFilename = filename;
}

INLINE const Bound& World::getBound(int index) const
{
   ASSERT(index < bounds.size())
   return *bounds[index];
}

INLINE Bound& World::getBound(int index)
{
   return const_cast<Bound&>(me().getBound(index));
}

INLINE int World::getBoundCount() const
{
   return bounds.size();
}

/// \fn World::getBounds() const
/// \brief Returns the bounds of this world.
INLINE const Bounds& World::getBounds() const
{
   return bounds;
}

INLINE Bounds& World::getBounds()
{
   return bounds;
}

/// \fn World::getLayer(int index)
/// \brief Returns a specific layer.
INLINE Layer* World::getLayer(int index)
{
   return const_cast<Layer*>(me().getLayer(index));
}

INLINE const Layer* World::getLayer(int index) const
{
   ASSERT_MSG(index >= 0 && index < layers.size(), "World::getLayer = Invalid layer index.");
   return layers[index];
}

INLINE int World::getLayerCount() const
{
   return layers.size();
}

INLINE int World::getObjectLayer() const
{
   return _objectLayer;
}

INLINE LayerType World::getLayerType() const
{
   return _layerType;
}

INLINE void World::setLayerType(int type)
{
   _layerType = (LayerType)type;
}

INLINE SimulationFactory& World::getSimulationFactory()
{
   ASSERT_PTR(mpSimulationFactory);
   return *mpSimulationFactory;
}

INLINE Simulator& World::getSimulator()
{
   ASSERT_PTR(mpSimulator);
   return *mpSimulator;
}

INLINE bool World::hasScript() const
{
   return mpScript != NULL;
}

INLINE c2d::ScriptObject& World::getScript()
{
   ASSERT_PTR(mpScript);
   return *mpScript;
}
   
INLINE void World::setScript(c2d::ScriptObject* pscript)
{
   mpScript = pscript;
}

//////////////////////////////////////////////////////////////////////////
// - Self
//////////////////////////////////////////////////////////////////////////

INLINE const World& World::me()
{
   return static_cast<const World&>(*this);
}
