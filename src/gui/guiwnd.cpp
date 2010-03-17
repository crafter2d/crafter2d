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
#include "guiwnd.h"
#ifndef JENGINE_INLINE
#  include "guiwnd.inl"
#endif

#include <GL/GLee.h>
#include <algorithm>

#include "../game.h"
#include "../scriptmanager.h"

#include "guicanvas.h"
#include "guidesigner.h"
#include "guiclipper.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"
#include "guiproperty.h"
#include "guifocus.h"
#include "guimanager.h"
#include "guiscopedtransform.h"

const GuiId InvalidId = 0;

void GuiWnd::initializeProperties()
{
   CONSTRUCT_STR_PROPERTY(Name, "Name", "")
   CONSTRUCT_STR_PROPERTY(Caption, "Caption", "");
   CONSTRUCT_STR_PROPERTY_WITH_CALLBACK(BackgroundImage, "Background image", "", onBackgroundChanged)
   CONSTRUCT_BOOL_PROPERTY(Background, "Background", true)
   CONSTRUCT_BOOL_PROPERTY(Border, "Border", true)
   CONSTRUCT_BOOL_PROPERTY(Visible, "Visible", true)
}

void GuiWnd::onBackgroundChanged(GuiWnd* pwnd)
{
   GuiProperty<std::string>* pprop = dynamic_cast<GuiProperty<std::string>*>(pwnd->getProperties().find("Background image"));
   pwnd->loadBackgroundImage(pprop->Value().c_str());
}

GuiWnd::GuiWnd():
   MProperties(),
   INIT_PROPERTY(Name),
   INIT_PROPERTY(Caption),
   INIT_PROPERTY(BackgroundImage),
   INIT_PROPERTY(Background),
   INIT_PROPERTY(Border),
   INIT_PROPERTY(Visible),
   _peventhandlerdefinitions(NULL),
   _peventhandlers(NULL),
   mObservers(),
   mpLayoutManager(NULL),
   m_frameRect(),
   parent(NULL),
   childs(),
   m_style(0),
   m_id(0),
   font(NULL),
   background()
{
   MProperties.setOwner(this);
}

GuiWnd::~GuiWnd()
{
	//destroy();
}

/*! \fn GuiWnd::create(GuiId id, GuiRect& rect, const char* caption="", GuiStyle style=0, GuiWnd* parent=NULL)
	\brief Initializes the window with the given information.
	\param id Id of window
	\param rect Position and size of window
	\param caption Caption of the window
	\param style Currently unused, must be 0
	\param parent Pointer to parent window, may be NULL

	This function initializes the window object with the values given as parameter. If a pointer to
	the parent is supplied, this object is linked as child of the parent.
 */
void GuiWnd::create (GuiId id, const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   font = GuiManager::getInstance().getDefaultFont();
	m_id = id;

   initializeProperties();
   initializeEventHandlerDefinitions();

   _peventhandlerdefinitions->registerAll(*this);

	onCreate(rect, caption, style, parent);

   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiWndCreatedEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.prepareCall(phandler->getFunctionName().c_str());
      script.run(0);
   }
}

void GuiWnd::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   m_frameRect = rect;
   m_style = style;

   setBorder(IS_SET(style, GUI_BORDER));
   setBackground(IS_SET(style, GUI_BACKGROUND));
   setVisible(IS_SET(style, GUI_VISIBLE));

   setCaption(caption);
	setParent(parent);
}

/*! \fn GuiWnd::destroy()
	\brief Destroys all child windows.

	When the window is closed or discarded the destroy function should be called to clean up
	the memory and free up the child controls.
 */
void GuiWnd::destroy()
{
   MProperties.clear();

   if ( !IS_SET(m_style, GUI_NODESTROY) )
   {
	   GuiList::Iterator it = childs.begin();
      for ( ; it.valid(); ++it)
      {
         GuiWnd* pwnd = (*it);

         pwnd->destroy();
		   delete pwnd;
      }
   }

   notifyDestroyed();
}

/*! \fn GuiWnd::setParent(GuiWnd* parent)
	\brief Set and link this object to the parent.
	\param parent The new parent of this window.

	To set a new parent call this function. It first detaches it from the current parent, if it
	had one. Then this window is added as child to the new parent.
 */
void GuiWnd::setParent (GuiWnd* p)
{
	if ( p != parent )
   {
		if (parent) {
			// remove old reference
         parent->removeChild(this);
		}

		// now link to new parent
		parent = p;
		if ( parent )
			parent->addChild(this);
	}
}

/*! \fn GuiWnd::addChild(GuiWnd* child)
	\brief Add the a new child window in the list
	\param child Child window to be added.

	If the child window doesn't have this window as it's parent, the GuiWnd::setParent function is
	called to set this as the parent window. Then the window is added to the childs list.
 */
void GuiWnd::addChild (GuiWnd* wnd)
{
	// make sure we don't create a cycle (setParent calls addChild and v.v.)
	if (wnd->parent != this)
		wnd->setParent (this);

   if (wnd->getStyle() & GUI_TOPMOST)
      childs.add(wnd);
   else
      childs.addTail(wnd);
}

/// \fn GuiWnd::removeChild(GuiWnd* wnd)
/// \brief Removes a child from the list
/// \param The child to remove
void GuiWnd::removeChild(GuiWnd* wnd)
{
   GuiList::Iterator it = childs.find(wnd);
   childs.removeAt(it);
}

bool GuiWnd::isChild(GuiWnd* pwnd) const
{
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiWnd* pchild = (*it);
      if ( pchild == pwnd || pchild->isChild(pwnd) )
         return true;
   }

   return false;
}

GuiWnd* GuiWnd::findChildByName(const std::string& name)
{
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiWnd* pchild = (*it);
      if ( pchild->getName() == name )
         return pchild;
   }

   return NULL;
}

void GuiWnd::loadBackgroundImage(const char* name)
{
   if (background.isValid())
      background.release();

   if ( strlen(name) > 0 )
   {
      background.load(name);

      setBackground(true);
   }
}

void GuiWnd::setWindowPos(int x, int y)
{
   int offx = x-m_frameRect.left(), offy = y-m_frameRect.top();
   moveWindow(offx, offy);
}

void GuiWnd::center()
{
   int sw, sh;

   int height = m_frameRect.getHeight();
   int width = m_frameRect.getWidth();
   Game::getInstance().getWindowDimensions(sw, sh);

   int offsX = ((sw/2) - (width/2)) - m_frameRect.left();
   int offsY = ((sh/2) - (height/2)) - m_frameRect.top();
   m_frameRect.offset (offsX, offsY);
}

GuiWnd* GuiWnd::getItemById (GuiId id) const
{
	GuiList::Iterator it = childs.begin();
	for ( ; it.valid(); ++it )
   {
		if ((*it)->m_id == id)
			return (*it);
   }

	return NULL;
}

//-----------------------------------
// - Rendering
//-----------------------------------

void GuiWnd::render(Uint32 tick, const GuiGraphics& graphics)
{
   if ( getVisible() )
   {
      onRender(tick, graphics);
      renderChildren(tick, graphics);
   }
}

/// GuiWnd::onRender(Uint32 tick, const GuiGraphics& graphics)
/// \brief Called during the rendering of a window or control
void GuiWnd::onRender(Uint32 tick, const GuiGraphics& graphics)
{
   const GuiCanvas& canvas = graphics.canvas();
   if ( getBackground() )
   {
      if (background.isValid()) {
         graphics.drawImage(background, m_frameRect);
      }
      else {
         graphics.setColor(canvas.getDefaultColor(GuiCanvas::GuiWindowColor));
         graphics.drawRect(m_frameRect);
      }
   }

   if ( getBorder() )
   {
      graphics.setColor(canvas.getDefaultColor(GuiCanvas::GuiBorderColor));
      graphics.drawWireRect(m_frameRect);
   }

   graphics.setColor(1,1,1);
}

/// \fn GuiWnd::renderChildren(Uint32 tick, const GuiGraphics& graphics)
/// \brief Render the child windows of this window.
void GuiWnd::renderChildren(Uint32 tick, const GuiGraphics& graphics)
{
   GuiScopedTransform transform(*this);

	GuiList::Iterator it = childs.end();
   for ( ; it.valid(); --it)
   {
      GuiWnd* pwnd = (*it);
	   pwnd->render (tick, graphics);
   }
}

void GuiWnd::sendMessage(GuiId id, GuiEvent event, int param1)
{
}

//////////////////////////////////////////////////////////////////////////
// - Event handler functionality
//////////////////////////////////////////////////////////////////////////

void GuiWnd::initializeEventHandlerDefinitions()
{
   _peventhandlerdefinitions = new GuiEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiWndCreatedEvent, "onCreated");
   _peventhandlerdefinitions->add(pdefinition);

   pdefinition = new GuiEventHandlerDefinition(GuiWndContextMenuEvent, "onContextMenu");
   pdefinition->addArgument("point");
   _peventhandlerdefinitions->add(pdefinition);

   pdefinition = new GuiEventHandlerDefinition(GuiWndContextCommandEvent, "onCommand");
   pdefinition->addArgument("id");
   _peventhandlerdefinitions->add(pdefinition);
}

void GuiWnd::initializeEventHandlers()
{
   _peventhandlers = new GuiEventHandlers();
}

void GuiWnd::registerEventHandler(GuiEventHandler* phandler)
{
   getEventHandlers().add(phandler);
}

//////////////////////////////////////////////////////////////////////////
// - Layout interface
//////////////////////////////////////////////////////////////////////////

GuiSize GuiWnd::getMinimumSize() const
{
   return virMinimumSize();
}

GuiSize GuiWnd::virMinimumSize() const
{
   return GuiSize();
}

GuiSize GuiWnd::getPreferredSize() const
{
   return virPreferredSize();
}

GuiSize GuiWnd::virPreferredSize() const
{
   return GuiSize();
}

void GuiWnd::invalidate()
{
}

void GuiWnd::validate()
{
}

//-----------------------------------
// - Listeners
//-----------------------------------

void GuiWnd::addKeyListener(KeyListener& listener)
{
  mKeyListeners.addListener(listener);
}

void GuiWnd::removeKeyListener(const KeyListener& listener)
{
  mKeyListeners.removeListener(listener);
}

void GuiWnd::fireKeyEvent(const KeyEvent& event)
{
  mKeyListeners.fireKeyEvent(event);
}

//-----------------------------------
// - Notification interface
//-----------------------------------

void GuiWnd::attach(GuiWndObserver& observer)
{
   Observers::iterator it = std::find(mObservers.begin(), mObservers.end(), &observer);

   if ( it == mObservers.end() )
   {
      mObservers.push_back(&observer);
   }
}

void GuiWnd::detach(GuiWndObserver& observer)
{
   Observers::iterator it = std::find(mObservers.begin(), mObservers.end(), &observer);
   ASSERT_MSG(it != mObservers.end(), "Observer is not attached!");

   mObservers.erase(it);
}

void GuiWnd::notifyDestroyed()
{
   for ( int index = 0; index < mObservers.size(); ++index )
   {
      GuiWndObserver& observer = *mObservers[index];
      observer.notifyDestroyed(*this);
   }
}

//-----------------------------------
// - Input interface
//-----------------------------------

void GuiWnd::onMouseMove (const GuiPoint& point, const GuiPoint& rel, int flags)
{
}

void GuiWnd::onMouseWheel (const GuiPoint& point, int direction, int flags)
{
}

int GuiWnd::onLButtonDown (const GuiPoint& point, int flags)
{
   return JENGINE_MSG_UNHANDLED;
}

int GuiWnd::onLButtonUp (const GuiPoint& point, int flags)
{
   return JENGINE_MSG_UNHANDLED;
}

int GuiWnd::onRButtonDown(const GuiPoint& point, int flags)
{
   return JENGINE_MSG_UNHANDLED;
}

void GuiWnd::onKeyDown (int which, bool shift, bool ctrl, bool alt)
{
}

void GuiWnd::onKeyUp (int which)
{
}

void GuiWnd::onActivate (bool active, GuiWnd* oldWnd)
{
	if (active)
		m_style |= GUI_ACTIVE;
	else
		m_style &= ~GUI_ACTIVE;
}

GuiWnd* GuiWnd::hitTest (const GuiPoint& point)
{
   if ( getVisible() && getWindowRect().pointInRect(point) )
   {
		GuiWnd* pwnd = this;
      GuiPoint cs(point);
		cs.x -= m_frameRect.left();
		cs.y -= m_frameRect.top();

		GuiList::Iterator it = childs.begin();
		for ( ; it.valid(); ++it)
      {
         GuiWnd* pchild = (*it);
			GuiWnd* pchildwnd = pchild->hitTest(cs);

         if ( pchildwnd != NULL )
         {
            pwnd = pchildwnd;
            break;
         }
		}

      return pwnd;
	}

	return NULL;
}

bool GuiWnd::onContextMenu(const GuiPoint& point)
{
   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiWndContextMenuEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiWnd");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam((void*)&point, "GuiPoint");
      script.run(1, 1);

      return script.getBoolean();
   }

   return false;
}

void GuiWnd::onCommand(int cmd)
{
   std::string name = GuiDesigner::controlName(this);
   if ( !name.empty() )
   {
      GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiWndContextCommandEvent);
      if ( phandler != NULL )
      {
         ScriptManager& mgr = ScriptManager::getInstance();
         Script& script = mgr.getTemporaryScript();
         script.setSelf(this, name.c_str());
         script.prepareCall(phandler->getFunctionName().c_str());
         script.addParam(cmd);
         script.run(1);
      }
   }
}

void GuiWnd::clientToWindow(GuiRect& rect) const
{
   GuiWnd* p = parent;
   while (p) {
      const GuiRect& pr = p->getWindowRect();
      rect.offset(pr.left(), pr.top());
      p = p->parent;
   }
}

void GuiWnd::windowToClient(GuiRect& rect) const
{
   const GuiWnd* p = this;
   do {
      const GuiRect& pr = p->getWindowRect();
      rect.offset(-pr.left(), -pr.top());
      p = p->parent;
   }
   while (p);
}

void GuiWnd::windowToClient(GuiPoint& point)
{
   GuiWnd* p = this;
   do
   {
      const GuiRect& pr = p->getWindowRect();
      point.x -= pr.left();
      point.y -= pr.top();
      p = p->parent;
   } while ( p );
}

bool GuiWnd::isBoundaryWnd() const
{
   return false;
}

GuiWnd* GuiWnd::getBoundaryParent()
{
   if ( getParent() )
      return getParent()->getBoundaryParent();
   else
      return NULL;
}

int GuiWnd::getTypeId() const
{
   return GuiInvalidId;
}

bool GuiWnd::hasFocus() const
{
   if ( (m_style & GUI_FOCUSED) == GUI_FOCUSED )
      return true;
   else
   {
      GuiList::Iterator it = childs.begin();
		for ( ; it.valid(); ++it)
      {
         GuiWnd* pchild = (*it);
         if ( pchild->hasFocus() )
            return true;
      }

      return false;
   }
}

void GuiWnd::setFocus()
{
   GuiFocus::getInstance().setFocus(this);
}

void GuiWnd::onSetFocus(GuiWnd* oldCtrl)
{
   m_style |= GUI_FOCUSED;
}

void GuiWnd::onKillFocus(GuiWnd* newCtrl)
{
   m_style &= ~GUI_FOCUSED;

   if ( IS_SET(m_style, GUI_NOTIFYPARENT) && hasParent() )
   {
      getParent()->sendMessage(getId(), GuiLostFocusEvent);
   }
}
