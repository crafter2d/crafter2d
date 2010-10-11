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
#ifndef GUIWND_H_
#define GUIWND_H_

#include "engine/texture.h"

#include "gui/input/keylisteners.h"
#include "gui/input/mouselisteners.h"
#include "gui/input/mousewheellisteners.h"

#include "wnd/guiwndmouselistener.h"

#include "guigraphics.h"
#include "guilist.h"
#include "guidefines.h"
#include "guihelpers.h"
#include "guiproperties.h"
#include "guiproperty.h"
#include "guiwndobserver.h"

extern const GuiId InvalidId;

class GuiEventHandlerDefinitions;
class GuiEventHandler;
class GuiEventHandlers;
class GuiFont;

class LayoutManager;

/*! @author Jeroen Broekhuizen
 
@section guiWnd_intro Introduction
JEngine is build on top of a complete GUI system. This system enables a designer to create custom windows, 
dialogs and controls. The GuiWnd class it the base class for this system and provides basic window functionality
like drawing, visibility, parent-child ordering, etc. 

@section guiWnd_lua Lua support
The creation and management of these windows is can be done in Lua script completely. Below an example implementation
is given of a simple dialog created in Lua script. The script below creates a simple window and centers it on
the client screen space.

@code
-- creation of the dialog box
wnd = GuiWnd:new()
wnd:create(1, GuiRect:new(0,400,0,75), "Window - this text is not visible")
wnd:center()
 
-- display the window on screen
canvas:pushWindow(wnd)
@endcode

To add a button the following code should follow:

@code
button = GuiButton:new()
button:create(3, GuiRect:new(150,250,30,45), "Close", 0, wnd)
button:setCommand("onClose()")
@endcode

The result is a rather large button in the center of the window. When it is pressed the command given with the
GuiButton::setCommand function is executed. In this case an onClose Lua function is called which should close the window 
by calling the GuiCanvas::popWindow function.
 */
class GuiWnd
{
public:
 // typedefs
   typedef std::vector<GuiWndObserver*> Observers;

 // statics
   static void onBackgroundChanged(GuiWnd* pwnd);

	                    GuiWnd();
   virtual           ~GuiWnd();

   void              create(GuiId id, const GuiRect& rect, const char* caption = "", GuiStyle style=0, GuiWnd* parent=NULL);
   virtual void      destroy();
 
  // rendering
   void              render(Uint32 tick, const GuiGraphics& graphics);

   void              invalidate();
   void              validate();

  // maintenance
   void              addChild(GuiWnd* wnd);
   bool              isChild(GuiWnd* pwnd) const;
   GuiWnd*           findChildByName(const std::string& name);
   void              removeChild(GuiWnd* wnd);   
   
  // get/set
   GuiId              getId() const;
   GuiStyle           getStyle() const;
   GuiWnd*            getItemById(GuiId id) const;

   const GuiRect&     getWindowRect() const;
   void               setWindowRect(const GuiRect& rect);
   
   GuiFont*           getFont() const;
   void               setFont(GuiFont* f);

   bool               hasParent() const;
   GuiWnd*            getParent() const;
   void               setParent(GuiWnd* p);

   virtual bool       isBoundaryWnd() const;
   virtual GuiWnd*    getBoundaryParent();

   bool               hasFocus() const;
   void               setFocus();

   bool               isTopmost() const;
   void               setTopmost(bool topmost);

   bool               getDestroyOnClose() const;
   void               setDestroyOnClose(bool destroy);

   GuiEventHandlerDefinitions&   getEventHandlerDefinitions();
   GuiEventHandlers&             getEventHandlers();
   GuiProperties&                getProperties();

  // observers
   void attach(GuiWndObserver& observer);
   void detach(GuiWndObserver& observer);

  // key listeners
   void addKeyListener(KeyListener& listener);
   void removeKeyListener(const KeyListener& listener);

   void fireKeyEvent(const KeyEvent& event);

  // mouse listeners
   void            addMouseListener(MouseListener& listener);
   void            removeMouseListener(const MouseListener& listener);
   MouseListeners& getMouseListeners() { return mMouseListeners; }

  // mouse wheel listeners
   void addMouseWheelListener(MouseWheelListener& listener);
   void removeMouseWheelListener(const MouseWheelListener& listener);

   void fireMouseWheelEvent(const MouseEvent& event);

  // coordinate conversions
   void              clientToWindow(GuiRect& rect) const;
   void              clientToWindow(GuiPoint& point) const;
   void              windowToClient(GuiRect& rect) const;
   void              windowToClient(GuiPoint& point);
   void              windowToClient(Point& point) const;

  // operations
   void              setWindowPos(int x, int y);
   void              resizeWindow(int w, int h);
   void              moveWindow(int x, int y);
   void              center();

   void              loadBackgroundImage(const char* name);

   void              registerEventHandler(GuiEventHandler* phandler);

   virtual void      sendMessage(GuiId, GuiEvent event, int param1=0);
	
  // callbacks
   virtual void      onActivate(bool active, GuiWnd* oldWnd);
   virtual void      onSetFocus(GuiWnd* oldCtrl);
   virtual void      onKillFocus(GuiWnd* newCtrl);
   virtual void      onCommand(int cmd);

   virtual GuiWnd*   hitTest(const GuiPoint& point);
   virtual int       getTypeId() const;

 // layout
   GuiSize           getMinimumSize() const;
   GuiSize           getPreferredSize() const;

protected:
   virtual void      onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void      onRender(Uint32 tick, const GuiGraphics& graphics);
   virtual void      onResize(int width, int height);
   virtual void      renderChildren(Uint32 tick, const GuiGraphics& graphics);
   
   virtual void      initializeProperties();
   virtual void      initializeEventHandlerDefinitions();
           void      initializeEventHandlers();

 // layout
   virtual GuiSize   virMinimumSize() const;
   virtual GuiSize   virPreferredSize() const;

 // notifications
           void      notifyDestroyed();

   GuiProperties  MProperties;
   STR_PROPERTY(Name)
   STR_PROPERTY(Caption)
   STR_PROPERTY(BackgroundImage)
   BOOL_PROPERTY(Background)
   BOOL_PROPERTY(Border)
   BOOL_PROPERTY(Visible)

protected:
   GuiEventHandlerDefinitions*   _peventhandlerdefinitions;
   GuiEventHandlers*             _peventhandlers;

   KeyListeners         mKeyListeners;
   MouseListeners       mMouseListeners;
   MouseWheelListeners  mMouseWheelListeners;

   GuiWndMouseListener  mwndMouseListener;

   Observers      mObservers;
   LayoutManager* mpLayoutManager;
   GuiRect        m_frameRect;
   GuiWnd*        parent;
   GuiList        childs;
   GuiStyle       m_style;
   GuiId          m_id;
   GuiFont*       font;
   Texture        background;

   bool mDestroyOnClose;
};

#ifdef JENGINE_INLINE
#  include "guiwnd.inl"
#endif

#endif
