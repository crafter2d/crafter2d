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
#ifndef GRID_AXIS_H_
#define GRID_AXIS_H_

#include "gridaxiscontrols.h"

class GridAxes;
class GridColumn;
class GridRow;

class GridAxis
{
public:
   explicit GridAxis(GridAxes& axes);
   virtual ~GridAxis();

 // get/set
   GridAxes&   getOwner();

   int   getIndex() const;
   void  setIndex(int index);

   float getOrigin() const;
   void  setOrigin(float origin);

   float getSize() const;
   void  setSize(float size);

   float getEnd() const;

   int   getGrowWeight() const;
   void  setGrowWeight(int weight);

   bool  grows() const;

 // downcast
   virtual bool         isColumn() const;
   virtual GridColumn&  asColumn();

   virtual bool         isRow() const;
   virtual GridRow&     asRow();

 // operations
   GridAxis*  split(float size);
   void       registerControl(GridControl& control);

   void       calculatePreferredSize();

protected:
 // construction
   virtual GridAxis* virConstruct() = 0;

 // get/set
   GridAxisControls& getControls();
   const GridAxisControls& getControls() const;

 // querying
   virtual float     virCalculatePreferredSize() const = 0;

private:
   GridAxisControls mControls;
   GridAxes&        mOwner;
   int              mIndex;
   float            mOrigin;
   float            mSize;
   float            mPreferredSize;
   int              mGrowWeight;
};

#ifdef JENGINE_INLINE
#  include "gridaxis.inl"
#endif

#endif // GRID_AXIS_H_
