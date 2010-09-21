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
#include "guidialog.h"

#include <string.h>

#include "gui/guiclipper.h"
#include "gui/guifont.h"
#include "gui/guigraphics.h"
#include "gui/guieventhandlerdefinition.h"
#include "gui/guieventhandlerdefinitions.h"
#include "gui/guitext.h"

#include "game.h"

IMPLEMENT_PROPERTIES(GuiDialog, GuiWindow)
   CONSTRUCT_BOOL_PROPERTY(Center, "Center", true)
   CONSTRUCT_STR_PROPERTY(OkButton, "Ok Button", "")
   CONSTRUCT_STR_PROPERTY(CancelButton, "Cancel Button", "")
}

GuiDialog::GuiDialog():
   GuiWindow(),
   INIT_PROPERTY(Center),
   INIT_PROPERTY(OkButton),
   INIT_PROPERTY(CancelButton),
   _tex(),
   _close(),
   _closepressed(),
   mKeyListener(*this),
   mMouseListener(*this),
   mHoverClose(false),
   _modalResult(0),
   _modal(false)
{
}

void GuiDialog::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWindow::onCreate(rect, caption, style, parent);

   // load the round mask texture for the dialog header
   _tex           = ResourceManager::getInstance().loadSystemTexture("circle.png");
   _closepressed  = ResourceManager::getInstance().loadSystemTexture("cancel.png");
   _close         = ResourceManager::getInstance().loadSystemTexture("cancel_disabled.png");

   if ( getCenter() )
      center();

   addKeyListener(mKeyListener);
   addMouseListener(mMouseListener);
}

void GuiDialog::initializeEventHandlerDefinitions()
{
   GuiWindow::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiDialogLoadedEvent, "onLoaded");

   getEventHandlerDefinitions().add(pdefinition);
}

// - Get/set interface

bool GuiDialog::isHoveringCloseButton() const
{
   return mHoverClose;
}
   
void GuiDialog::setHoverCloseButton(bool hover)
{
   mHoverClose = hover;
}

// - Query interface

bool GuiDialog::isAboveTitleBar(const Point& point) const
{
   int right = getWindowRect().getWidth() - 10;
   GuiRect rect(10, right, -7, 9);
   return rect.pointInRect(GuiPoint(point));
}

bool GuiDialog::isAboveCloseButton(const Point& point) const
{
   int right = getWindowRect().getWidth() - 10;
   GuiRect btn(right-14, right-2, -5, 7);
   return btn.pointInRect(GuiPoint(point));
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiDialog::onRender(Uint32 tick, const GuiGraphics& graphics)
{
   GuiClipper clipper(getWindowRect());

   GuiWnd::onRender(tick, graphics);
   renderCaption(graphics);
}

void GuiDialog::renderCaption(const GuiGraphics& graphics)
{
   static const GuiRect lefthalf(0, 0.5, 0, 1);
   static const GuiRect righthalf(0.5, 1, 0, 1);

   float height = Game::getInstance().getCanvas().getWindowRect().getHeight();
   float left = m_frameRect.left()+10;
   float top  = m_frameRect.top()-7;
   float right = m_frameRect.right()-10;
   int fontheight = font->getHeight();

   // make sure we can render the caption
   glScissor(left-1, height-(top+fontheight+2), (right-left)+2, 19);

   // draw the rectangle
   glColor4f (0.601f, 0.601f, 0.601f, 1);
   graphics.drawImage(*_tex, lefthalf, GuiRect(left,left+8, top,top+16));
   glRectf(left+8,top+16,right-8,top);
   graphics.drawImage(*_tex, righthalf, GuiRect(right-8,right, top,top+16));

   if ( mHoverClose )
      graphics.drawImage(*_closepressed, GuiRect(right-14, right-2, top+2, top+14));
   else
      graphics.drawImage(*_close, GuiRect(right-14, right-2, top+2, top+14));

   // draw the caption
   glColor3f(0,0,0);
   GuiText::printfn(*font, left+5, top+font->getBaseHeight(), getCaption());
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

bool GuiDialog::isModal() const
{
   return _modal;
}

int GuiDialog::doModal()
{
   Game& game = Game::getInstance();
   game.getCanvas().doModal(this);
   setFocus();

   _modal = true;
   while ( isModal() && game.isActive() )
   {
      game.processFrame();
   }

   if ( game.isActive() )
      return _modalResult;
   else
      return -1;
}

void GuiDialog::endModal(int retValue)
{
   if ( isModal() )
   {
      _modalResult = retValue;
      _modal       = false;
   }
}

void GuiDialog::close(bool ok)
{
   GuiCanvas& canvas = Game::getInstance().getCanvas();

   if ( isModal() )
   {
      GuiButton* pbutton = dynamic_cast<GuiButton*>(findChildByName(ok ? getOkButton() : getCancelButton()));
      if ( pbutton != NULL )
         pbutton->click();
      else if ( ok )
         canvas.endModal(0);
      else
         canvas.endModal(1);
   }
   else
      canvas.popWindow(this);
}