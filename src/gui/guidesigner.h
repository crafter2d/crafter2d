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
#ifndef GUIDESIGNER_H
#define GUIDESIGNER_H

#include <string>
#include <vector>

#include "guiwindow.h"

#include "designer/designerfocuslistener.h"
#include "designer/guidesignermouselistener.h"

class GuiDesignWnd;
class GuiEventHandlerDlg;
class GuiDialog;
class GuiPoint;
class GuiMenu;
class GuiDesignMenu;
class GuiPopupDecorator;

typedef GuiControl* (*createControlFnc)(int id, const GuiPoint& pos, GuiWnd* pparent);

struct GuiRegisteredControl
{
   GuiRegisteredControl(int id, const std::string& ctrlname, const std::string& ctrldesc, createControlFnc function);

   std::string name;
   std::string _description;
   createControlFnc pfunction;
   int _id;
};

/*!
@author Jeroen Broekhuizen

@section guiCanvas_intro Introduction
Every game needs a user interface. From a simple screen with a couple of buttons to complex tab pages
with numerous controls on it. Creating an XML file for the simple screen is doable, but as soon as it 
becomes more complex, this task becomes tedious. The Designer helps you to create all your user interfaces
in a what-you-see-is-what-you-get (WYSIWYG) environment. With its easy to use interface you can create
complex user interface quickly.

@section guiCanvas_using Using the Designer
The designer can be started anytime by pressing the F3 key. Initialy the Designer will show up with
two dialogs and an empty window on which you can design your interface. You will be using the two
dialogs extensively during the designing phase. The first dialog is the Properties dialog. In this
dialog you can edit all the properties of a selected window or control. The other dialog is used to
edit the event handlers of the current selection.

@subsection guiCanvas_using_properties Editing properties
Every window, dialog and control (from now on I will refer to this set with windows only) has a set
of properties which can be edited to create your own unique user interface.

@subsection guiCanvas_using_eventhandlers Editing event handlers
Just as properties most windows also have event handlers (see 

\see GuiEventHandler Introduction.
*/
class GuiDesigner : public GuiWindow
{
public:
   typedef std::vector<GuiRegisteredControl> GuiRegisteredControls;

 // Control registration interface
   static GuiControl*            createControl(int controlid, int id, const GuiPoint& location, GuiWnd* parent);
   static int                    registerControl(int id, const std::string& name, const std::string& desc, createControlFnc pfunction);
   static GuiRegisteredControls& registeredControls();
   static std::string            controlName(GuiWnd* pcontrol);

            GuiDesigner();
   virtual ~GuiDesigner();

// Get/set interface
   GuiMenu&          getDesignerPopup();
   GuiMenu&          getWindowPopup();
   
   virtual void      sendMessage(GuiId id, GuiEvent event, int param1);
   virtual void      onCommand(int id);

// Operations
   void  close(GuiWnd* pwindow);
   void  focusChanged(GuiWnd& wnd);

protected:
// Overloads
   virtual void      onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);

private:
   friend class GuiDesignerMouseListener;

   static GuiRegisteredControl*  findControl(int id);

// Operations
   void           createDesignerPopup();
   void           createWindowPopup();
   void           createProjectWorkspace();
   void           createPropertyWnd();
   void           createEventHandlerWnd();
   GuiDesignWnd*  createWindow();
   GuiDesignMenu* createMenu();

   void  load();

 // Context menu
   void  onMenuLostFocus();

// Self
   const GuiDesigner& me();

// Member variables
   static GuiRegisteredControls* _registeredControls;

   GuiDialog*           _pprojectWorkspace;
   GuiDialog*           _ppropertyDlg;
   GuiDialog*           _peventhandlerdlg;
   GuiMenu*             _pdesignerPopup;
   GuiMenu*             _pwindowPopup;
   GuiPopupDecorator*   _popupMenu;

   DesignerFocusListener    mFocusListener;
   GuiDesignerMouseListener mMouseListener;
};

#ifdef JENGINE_INLINE
#  include "guidesigner.inl"
#endif

#endif
