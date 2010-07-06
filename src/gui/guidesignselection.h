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
#ifndef GUIDESIGNSELECTION_H
#define GUIDESIGNSELECTION_H

#include "guicontrol.h"

#include "designer/guidesignselectionmouselistener.h"
#include "designer/guidesignselectionmousemotionlistener.h"

class GuiDesignWnd;

/**
@author Jeroen Broekhuizen
*/
class GuiDesignSelection : public GuiControl
{
public:
   enum Sides { eLeft=1, eRight=2, eTop=4, eBottom=8, eNone=0 };

   GuiDesignSelection();
   virtual ~GuiDesignSelection();

 // Operations
   void  resize(const GuiPoint& rel, int borders);
    
 // Overloads
   virtual GuiWnd*   hitTest(const GuiPoint& point);

protected:
 // creation
   virtual void   onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);

 // rendering
   virtual void   onRender(Uint32 tick, const GuiGraphics& graphics);

private:
   friend class GuiDesignWnd;
   friend class GuiDesignDecorator;
   friend class GuiDesignSelectionMouseListener;
   friend class GuiDesignSelectionMouseMotionListener;

   GuiDesignWnd*  getDesignWnd() const;

   GuiDesignSelectionMouseListener       mMouseListener;
   GuiDesignSelectionMouseMotionListener mMouseMotionListener;

   int mBorders;
};

#endif
