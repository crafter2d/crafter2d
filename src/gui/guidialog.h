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
#ifndef _GUIDIALOG_H_
#define _GUIDIALOG_H_

#include "../resources.h"
#include "guiwindow.h"
#include "guiimagebutton.h"

/*! @author Jeroen Broekhuizen
 
@section guiDialog_intro Introduction
Dialogs are a special kind of window. Dialogs can have captions and are moveable entities.
*/
class GuiDialog : public GuiWindow
{ 
public:
                  GuiDialog();

   virtual void   onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flag);
   virtual int    onLButtonDown(const GuiPoint& point, int flags);
   virtual int    onLButtonUp(const GuiPoint& point, int flags);
   virtual void   onKeyDown(int which, bool shift, bool ctrl, bool alt);

   bool  isModal() const;
   int   doModal();
   void  endModal(int retValue);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   onRender(Uint32 tick, const GuiGraphics& graphics);
   virtual void   initializeEventHandlerDefinitions();

   bool           isAboveCloseButton(const GuiPoint& point) const;
   void           close(bool ok);

   void           renderCaption(const GuiGraphics& graphics);

   DECLARE_PROPERTIES
   BOOL_PROPERTY(Center)
   STR_PROPERTY(OkButton)
   STR_PROPERTY(CancelButton)

   TexturePtr  _tex;
   TexturePtr  _close;
   TexturePtr  _closepressed;
   bool        _hoverClose;
   int         _modalResult;
   bool        _modal;
   bool        _dragging;
};

#endif
