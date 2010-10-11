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
#ifndef _GUIDIALOG_H_
#define _GUIDIALOG_H_

#include "engine/resources.h"

#include "gui/guiwindow.h"
#include "gui/guiimagebutton.h"

#include "guidialogkeylistener.h"
#include "guidialogmouselistener.h"

/*! @author Jeroen Broekhuizen
 
@section guiDialog_intro Introduction
Dialogs are a special kind of window. Dialogs can have captions and are moveable entities.
*/
class GuiDialog : public GuiWindow
{ 
public:
                  GuiDialog();

 // query interface
   bool isAboveTitleBar(const Point& point) const;
   bool isAboveCloseButton(const Point& point) const;

 // modal
   bool  isModal() const;
   int   doModal();
   void  endModal(int retValue);

protected:
   friend class GuiDialogKeyListener;
   friend class GuiDialogMouseListener;

 // get/set
   bool isHoveringCloseButton() const;
   void setHoverCloseButton(bool hoover);

  // overloads
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   onRender(Uint32 tick, const GuiGraphics& graphics);
   virtual void   initializeEventHandlerDefinitions();

  // operations
   void           close(bool ok);

  // rendering
   void           renderCaption(const GuiGraphics& graphics);

   DECLARE_PROPERTIES
   BOOL_PROPERTY(Center)
   STR_PROPERTY(OkButton)
   STR_PROPERTY(CancelButton)

   TexturePtr  _tex;
   TexturePtr  _close;
   TexturePtr  _closepressed;

   GuiDialogKeyListener         mKeyListener;
   GuiDialogMouseListener       mMouseListener;

   bool        mHoverClose;
   int         _modalResult;
   bool        _modal;
};

#endif
