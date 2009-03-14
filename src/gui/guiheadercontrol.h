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
#ifndef GUIHEADERCTRL_H_
#define GUIHEADERCTRL_H_

#include <vector>
#include "guicontrol.h"

class GuiHeaderColumn;

class GuiHeaderCtrl : public GuiControl
{
public:
   typedef std::vector<GuiHeaderColumn*> Columns;

                           GuiHeaderCtrl();
   virtual                 ~GuiHeaderCtrl();

   static int              height();

   bool                    create (GuiId id, GuiStyle style, GuiWnd* parent);

   int                     count() const;

   int                     addColumn(int width);
   void                    remove(int index);

   const GuiHeaderColumn&  operator[](int index) const;
   GuiHeaderColumn&        operator[](int index);

   virtual void            onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags);
   virtual int             onLButtonDown(const GuiPoint& point, int flags);
   virtual int             onLButtonUp(const GuiPoint& point, int flags);

protected:
   virtual void            paint(Uint32 tick, const GuiGraphics& graphics);

   bool                    isDragPoint(const GuiPoint& point);
   void                    dragging(bool drag);
   bool                    dragging() const;

   int                     calculateNewLeftSize();

private:
   Columns   columns;
   bool     _dragging;
   int      _dragcolumn;
};

#ifdef JENGINE_INLINE
#  include "guiheadercontrol.inl"
#endif

#endif
