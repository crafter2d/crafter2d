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
#ifndef _PICKVISITOR_H_
#define _PICKVISITOR_H_

#include "core/math/vector.h"

#include "nodevisitor.h"

/// Implements the pick visitor. Use it to determine if the cursor is above
/// an object.
class PickVisitor : public NodeVisitor
{
public:
            PickVisitor();
   explicit PickVisitor(const Vector& pos);

   void    setPickPos(Vector pos);
   Entity* getPicked();

 // visit
   virtual void visitEntity(Entity* pentity) override;

protected:

 // data
   Entity* mpPicked;
   Vector  mPickPos;
};

#ifdef JENGINE_INLINE
#  include "pickvisitor.inl"
#endif

#endif
