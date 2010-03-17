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
#include "../script.h"

#include "guicanvas.h"
#ifndef JENGINE_INLINE
#  include "guicanvas.inl"
#endif

#include "game.h"

#include "guifocus.h"
#include "guidialog/guidialog.h"
#include "guiclipper.h"
#include "guidesigner.h"
#include "guimanager.h"

GuiColor GuiCanvas::defaultColors[GuiCanvas::MaxGuiColors];

GuiCanvas::GuiCanvas():
   GuiWnd(),
   graphics(),
   windows(),
   modal(),
   activeWnd(),
   mpDesigner(NULL),
   mKeyDispatcher(*this),
   mMouseDispatcher(*this)
{
   graphics.canvas(this);
}

GuiCanvas::~GuiCanvas()
{
}

//////////////////////////////////////////////////////////////////////////
// - Static interface
//////////////////////////////////////////////////////////////////////////

// static
bool GuiCanvas::isShift()
{
   return (SDL_GetModState() & KMOD_SHIFT);
}

// static
bool GuiCanvas::isAlt()
{
   return (SDL_GetModState() & KMOD_ALT);
}

// static
bool GuiCanvas::isCtrl()
{
   return (SDL_GetModState() & KMOD_CTRL);
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////


void GuiCanvas::create(GuiId id, const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWnd::create(id, rect, caption, style, parent);

   GameWindow& window = Game::getInstance().getGameWindow();
   window.setKeyEventDispatcher(mKeyDispatcher);
   window.setMouseEventDispatcher(mMouseDispatcher);
}

void GuiCanvas::destroy()
{
   GuiWnd::destroy();
}

//////////////////////////////////////////////////////////////////////////
// - Painting
//////////////////////////////////////////////////////////////////////////

/// \fn GuiCanvas::render(Uint32 tick)
/// \brief Render all windows on this canvas.
void GuiCanvas::render(Uint32 tick)
{
   GuiClipper clipper(getWindowRect());

   GuiList::Iterator it = windows.end ();
   for ( ; it.valid(); --it)
   {
      GuiWnd* pwnd = (*it);
      if ( pwnd->getVisible() )
         pwnd->render(tick, graphics);
   }

   it = modal.end();
   for ( ; it.valid(); --it)
   {
      GuiWnd* pwnd = (*it);
      pwnd->render(tick, graphics);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Input messages
//////////////////////////////////////////////////////////////////////////

void GuiCanvas::onMouseMove(const GuiPoint& point, const GuiPoint& rel, const int flag)
{
   GuiList::Iterator it = modal.begin();
   if ( !it.valid() )
   {
      if ( GuiFocus::getInstance().hasFocus() )
      {
         GuiPoint p(point);
         GuiWnd& focusWnd = GuiFocus::getInstance().getFocus();
         focusWnd.windowToClient(p);

         GuiFocus::getInstance().getFocus().onMouseMove(p, rel, flag);
      }
   }
   else if ( GuiFocus::getInstance().hasFocus() )
   {
      GuiWnd* wnd = (*it);
      GuiPoint p(point);
      GuiWnd& focusWnd = GuiFocus::getInstance().getFocus();
      focusWnd.windowToClient(p);

      focusWnd.onMouseMove(p, rel, flag);
   }
}

void GuiCanvas::onMouseWheel(const GuiPoint& point, int direction, int flag)
{
   if ( modal.empty() )
   {
      // no modal dialog on top
      GuiList::Iterator it = windows.begin();
      for (; it.valid(); ++it) {
         GuiWnd* wnd = (*it)->hitTest (point);
         if (wnd)
         {
            wnd->onMouseWheel(point, direction, flag);
            break;
         }
      }
   }
   else
   {
      // redirect input directly to modal dialog
      GuiWnd* pmodelWnd = *modal.begin();
      GuiWnd* pwnd = pmodelWnd->hitTest(point);
      if ( pwnd != NULL )
         return pwnd->onMouseWheel(point, direction, flag);
   }
}

int GuiCanvas::onLButtonDown(const GuiPoint& point, int flag)
{
   findFocusUnderCursor(point);

   GuiFocus& focus = GuiFocus::getInstance();
   if ( focus.hasFocus() )
   {
      GuiPoint p(point);
      GuiWnd& focusWnd = focus.getFocus();
      focusWnd.windowToClient(p);

      return focusWnd.onLButtonDown(p, flag);
   }

   return JENGINE_MSG_UNHANDLED;
}

int GuiCanvas::onLButtonUp(const GuiPoint& point, int flag)
{
   GuiFocus& focus = GuiFocus::getInstance();

   if ( !focus.hasFocus() || !focus.getFocus().hitTest(point) )
   {
      // if the currently focused window is not selected anymore with the mouse,
      // try to find the new window
      findFocusUnderCursor(point);
   }

   if ( focus.hasFocus() )
   {
      GuiPoint p(point);
      GuiWnd& focusWnd = focus.getFocus();
      focusWnd.windowToClient(p);

      return focusWnd.onLButtonUp(p, flag);
   }

   return JENGINE_MSG_UNHANDLED;
}

int GuiCanvas::onRButtonDown (const GuiPoint& point, int flag)
{
   findFocusUnderCursor(point);

   GuiFocus& focus = GuiFocus::getInstance();
   if ( focus.hasFocus() )
   {
      if ( !focus.getFocus().onContextMenu(point) )
         return focus.getFocus().onRButtonDown(point, flag);
      else
         return JENGINE_MSG_HANDLED;
   }

   return JENGINE_MSG_UNHANDLED;
}

void GuiCanvas::onKeyDown(int which, bool shift, bool ctrl, bool alt)
{
   GuiFocus& focus = GuiFocus::getInstance();
   if ( focus.hasFocus() )
   {
      focus.getFocus().onKeyDown(which, shift, ctrl, alt);
   }
}

void GuiCanvas::onKeyUp (int which)
{
   switch ( which )
   {
   case SDLK_F3:
      switchDesigner();
      break;
   case SDLK_F4:
      switchEditor();
      break;
   default:
      {
         GuiFocus& focus = GuiFocus::getInstance();
         if ( focus.hasFocus() )
         {
            focus.getFocus().onKeyUp(which);
         }
      }
   }
}

void GuiCanvas::findFocusUnderCursor(const GuiPoint& point)
{
   GuiFocus& focus = GuiFocus::getInstance();
   if ( modal.empty() )
   {
      GuiList::Iterator it(windows.begin());
      for ( ; it.valid(); ++it)
      {
         GuiWnd* pwindow = *it;
         GuiWnd* pwnd = pwindow->hitTest(point);
         if ( pwnd != NULL )
         {
            setActiveWnd(pwnd->getBoundaryParent());
            focus.setFocus(pwnd);
            break;
         }
      }
   }
   else
   {
      GuiWnd* pmodelWnd = *modal.begin();
      focus.setFocus(pmodelWnd->hitTest(point));
   }
}

//////////////////////////////////////////////////////////////////////////
// - Interface methods
//////////////////////////////////////////////////////////////////////////

/// \fn GuiCanvas::pushWindow(GuiWnd* pwnd)
/// \brief Pushes the g:iven window to the top of the stack (window does not
/// have to be visible.
void GuiCanvas::pushWindow(GuiWnd* wnd)
{
   windows.add(wnd);

   setActiveWnd(wnd);
   wnd->setFocus();
}

void GuiCanvas::popAll()
{
   windows.removeAll();
}

/// \fn GuiCanvas::popWindow()
/// \brief Removes the top window from screen.
void GuiCanvas::popWindow()
{
   doPopWindow(windows.begin());
}

/// \fn GuiCanvas::popWindow(GuiWnd* pwnd)
/// \brief Removes the given window from the window stack.
void GuiCanvas::popWindow(GuiWnd* pwnd)
{
   doPopWindow(windows.find(pwnd));
}

/// \fn GuiCanvas::doPopWindow(GuiList::Iterator it)
/// \brief Performs the actual removeal of the given iterator from the window stack.
void GuiCanvas::doPopWindow(GuiList::Iterator it)
{
   if ( it.valid() )
   {
      windows.removeAt(it);

      setActiveWnd(*windows.begin());
   }
}

/// \fn GuiCanvas::doModal(GuiWnd* pwnd)
/// \brief Displays the modal dialog, all input will be directed to this dialog,
/// until it is closed.
void GuiCanvas::doModal(GuiWnd* pwnd)
{
   modal.add(pwnd);

   setActiveWnd(pwnd);
}

/// \fn GuiCanvas::endModal()
/// \brief Removes the topmost modal dialog from the stack. It reactivates the
/// new topmost modal dialog or window.
void GuiCanvas::endModal(int retValue)
{
   // remove the window from the modal list
   GuiList::Iterator it = modal.begin();
   if ( it.valid() )
   {
      GuiDialog* pmodalDlg = dynamic_cast<GuiDialog*>(*it);
      if ( pmodalDlg != NULL )
         pmodalDlg->endModal(retValue);

      modal.removeHead();
      if ( modal.begin().valid() )
         setActiveWnd(*modal.begin());
      else if ( windows.count() > 0 )
         setActiveWnd(*windows.begin());
      else
         setActiveWnd(NULL);
   }
}

/// \fn GuiCanvas::setActiveWnd(GuiWnd* pwnd)
/// \brief Sets the new active window. The former active window will become
/// inactive.
void GuiCanvas::setActiveWnd(GuiWnd* pwnd)
{
   if ( !(activeWnd == *pwnd) )
   {
      GuiWnd* pold = activeWnd.instancePtr();
      if ( activeWnd.isAlive() )
         activeWnd->onActivate(false, pwnd);

      activeWnd = pwnd;
      if ( activeWnd.isAlive() )
         activeWnd->onActivate(true, pold);
   }
}

/// \fn GuiCanvas::setFocus(GuiWnd* pwnd)
/// \brief Sets the new focus window and signals both old and new wnd.
void GuiCanvas::setFocus(GuiWnd* pwnd)
{
   GuiFocus::getInstance().setFocus(pwnd);
}

/// \fn GuiCanvas::quit()
/// \brief Stops the application.
void GuiCanvas::quit()
{
   Game::getInstance().setActive(false);
}

// - Build-in applications

void GuiCanvas::switchDesigner()
{
   if ( isDesigning() )
   {
      mpDesigner = new GuiDesigner();
      mpDesigner->create(1, GuiRect(0,300,0,300), "Designer", 1420, this);
      mpDesigner->center();

      pushWindow(mpDesigner);
   }
   else
   {
      ASSERT_PTR(mpDesigner);

      popWindow(mpDesigner);

      mpDesigner->destroy();
      delete mpDesigner;
      mpDesigner = NULL;
   }
}

void GuiCanvas::switchEditor()
{
   GuiDialog* peditor = GuiManager::getInstance().loadDialogFromXML("te_editor");
   peditor->center();

   pushWindow(peditor);
}