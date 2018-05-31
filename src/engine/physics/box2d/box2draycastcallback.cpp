/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "box2draycastcallback.h"

#include "box2dsimulator.h"

Box2DRayCastCallback::Box2DRayCastCallback():
   b2RayCastCallback(),
   mpObject(nullptr),
   mPos(),
   mDistance(0)
{
}

// query
   
bool Box2DRayCastCallback::hasCollision() const
{
   return mpObject != nullptr;
}

// - Collision reporting
   
float32 Box2DRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
   mpObject = fixture;
   mPos = Box2DSimulator::b2ToVector(point);
   mDistance = fraction;

   return fraction; // clip and continue, find closest intersection
}
