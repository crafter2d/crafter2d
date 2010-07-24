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
#include "guicanvas.h"
#ifndef JENGINE_INLINE
#  include "guicanvas.inl"
#endif

#include "game.h"
#include "script.h"

#include "gui/guifocus.h"
#include "gui/guidialog/guidialog.h"
#include "gui/guiclipper.h"
#include "gui/guidesigner.h"
#include "gui/guimanager.h"

//-----------------------------------------------
// - Statics
//-----------------------------------------------

GuiColor GuiCanvas::defaultColors[GuiCanvas::MaxGuiColors];

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

//-----------------------------------------------
// - Class
//-----------------------------------------------

GuiCanvas::GuiCanvas():
   GuiWnd(),
   graphics(),
   windows(),
   modal(),
   activeWnd(),
   mpDesigner(NULL),
   mFocusListener(*this),
   mKeyDispatcher(*this),
   mMouseDispatcher(*this)
{
   graphics.canvas(this);
}

GuiCanvas::~GuiCanvas()
{
}

//-----------------------------------------------
// - Construction
//-----------------------------------------------


void GuiCanvas::create(GuiId id, const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWnd::create(id, rect, caption, style, parent);

   GuiFocus::getInstance().addListener(mFocusListener);

   GameWindow& window = Game::getInstance().getGameWindow();
   window.setKeyEventDispatcher(mKeyDispatcher);
   window.setMouseEventDispatcher(mMouseDispatcher);
}

void GuiCanvas::destroy()
{
   GuiWnd::destroy();
}

//-----------------------------------------------
// - Rendering
//-----------------------------------------------

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

//-----------------------------------------------
// - Searching
//-----------------------------------------------

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

//-----------------------------------------------
// - Notifications
//-----------------------------------------------

void GuiCanvas::notifyFocusChanged(GuiWnd& newfocus, GuiWnd* poldfocus)
{
   if ( poldfocus != NULL )
   {
      poldfocus->onKillFocus(&newfocus);
   }

   newfocus.onSetFocus(poldfocus);

   setActiveWnd(newfocus.getBoundaryParent());
}


//-----------------------------------------------
// - Interface methods
//-----------------------------------------------

bool GuiCanvas::isWindowDisplayed(GuiWnd* pwnd)
{
   GuiList::Iterator it = windows.find(pwnd);
   return it.valid();
}

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

//-----------------------------------------------
// - Finding
//-----------------------------------------------

GuiWnd* GuiCanvas::findWindowAtLocation(const Point& point)
{
   GuiWnd* pwindow = NULL;
   GuiPoint guipoint(point);
   GuiFocus& focus = GuiFocus::getInstance();

   if ( focus.hasFocus() )
   {
      GuiWnd& focussed = focus.getFocus();
      GuiPoint focuspoint(guipoint);
      
      if ( focussed.hasParent() )
         focussed.getParent()->windowToClient(focuspoint);

      pwindow = focussed.hitTest(focuspoint);
   }
   
   if ( pwindow == NULL )
   {
      if ( modal.empty() )
      {
         GuiList::Iterator it(windows.begin());
         for ( ; it.valid(); ++it )
         {
            GuiWnd* pwnd = (*it)->hitTest(guipoint);
            if ( pwnd != NULL )
            {
               pwindow = pwnd;
               break;
            }
         }
      }
      else
      {
         GuiWnd* pmodelWnd = *modal.begin();
         pwindow = pmodelWnd->hitTest(point);
      }
   }

   return pwindow;
}

//----------------------------------
// - Build-in applications
//----------------------------------

void GuiCanvas::switchDesigner()
{
   if ( !isDesigning() )
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