/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#ifndef GUIDESIGNDECORATOR_H
#define GUIDESIGNDECORATOR_H

#include "guicontrol.h"

class GuiDesignSelection;

/**
@author Jeroen Broekhuizen
*/
class GuiDesignDecorator : public GuiControl
{
public:
   GuiDesignDecorator();
   virtual ~GuiDesignDecorator();

 // Get/set interface
   const GuiControl&    control() const;
         GuiControl&    control();
   void                 control(GuiControl* pcontrol);

   GuiDesignSelection&  selectionCtrl();

   bool                 selected() const;
   void                 selected(bool select);

 // Overloads
   virtual GuiWnd*   hitTest(const GuiPoint &point);

   virtual void      onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags);
   virtual int       onLButtonDown (const GuiPoint& point, int flags);
   virtual int       onLButtonUp (const GuiPoint& point, int flags);
   
   virtual void      onKeyUp(int which);

   virtual bool      onContextMenu(const GuiPoint& point);
   virtual void      onCommand(int cmd);

protected:
   virtual void   onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   onRender(Uint32 tick, const GuiGraphics& graphics);
   virtual void   onResize(int width, int height);

private:
   const GuiDesignDecorator& me();

   GuiControl*          MPControl;
   GuiDesignSelection*  _selectionCtrl;
   bool                 MDragging;
};

#ifdef JENGINE_INLINE
#  include "guidesigndecorator.h"
#endif

#endif
