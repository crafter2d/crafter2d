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
#ifndef BOX2D_ROPE_JOINT_H_
#define BOX2D_ROPE_JOINT_H_

#include "core/math/vector.h"

#include "box2djoint.h"

class Box2DBody;
class b2RopeJoint;

class Box2DRopeJoint : public Box2DJoint
{
public:
   Box2DRopeJoint(b2RopeJoint& joint);

private:
   b2RopeJoint& mJoint;
};

#endif // BOX2D_ROPE_JOINT_H_
