/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef COLLISION_RESOLVER_INFO_H_
#define COLLISION_RESOLVER_INFO_H_

class CollisionResolverInfo
{
public:
   CollisionResolverInfo();
   CollisionResolverInfo(const CollisionResolverInfo& that);
   CollisionResolverInfo& operator=(const CollisionResolverInfo& that);

   int  getMaxPositionIterations() const;
   void setMaxPositionIterations(int max);

   int  getMaxVelocityIterations() const;
   void setMaxVelocityIterations(int max);

   float getPositionEpsilon() const;
   void  setPositionEpsilon(float epsilon);

   float getVelocityEpsilon() const;
   void  setVelocityEpsilon(float epsilon);

private:
   int   mMaxPositionIterations;
   int   mMaxVelocityIterations;
   float mPositionEpsilon;
   float mVelocityEpsilon;
};

#ifdef JENGINE_INLINE
#  include "collisionresolverinfo.inl"
#endif

#endif
