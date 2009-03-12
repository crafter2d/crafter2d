/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "guidesigner.h"
#ifndef JENGINE_INLINE
#  include "guidesigner.inl"
#endif

#include <tinyxml.h>

#include "../game.h"
#include "../gameconfiguration.h"
#include "../script.h"
#include "../scriptmanager.h"

#include "guidesignwnd.h"
#include "guidesigndecorator.h"
#include "guidesignmenu.h"
#include "guidialog.h"
#include "guiedit.h"
#include "guilist.h"
#include "guimenu.h"
#include "guipropertylist.h"
#include "guieventhandlerdlg.h"
#include "guipopupdecorator.h"
#include "guimanager.h"
#include "guiclipper.h"
#include "guiheadercolumn.h"

//DEFINE_CONSTRUCTOR(GuiDesigner)
//
//DEFINE_METHOD(GuiDesigner,activeWindow)
//   USER_ARG(GuiDesignWnd,2)
//DEFINE_END()
//
//REGISTER_CLASS(GuiDesigner,GuiWindow)
//   REGISTER_CONSTRUCTOR(GuiDesigner)
//   REGISTER_METHOD_1(GuiDesigner, activeWindow, GuiDesignWnd*)
//REGISTER_END(GuiDesigner)

EVENT_MAP_BEGIN(GuiDesigner)
   ON_LOST_FOCUS(24, onMenuLostFocus)
EVENT_MAP_END()

//////////////////////////////////////////////////////////////////////////
// - Control registration interface
//////////////////////////////////////////////////////////////////////////

GuiDesigner::GuiRegisteredControls* GuiDesigner::_registeredControls = NULL;

GuiRegisteredControl::GuiRegisteredControl(int id, const std::string& ctrlname, const std::string& ctrldesc, createControlFnc function):
   name(ctrlname),
   _description(ctrldesc),
   pfunction(function),
   _id(id)
{
}

GuiControl* GuiDesigner::createControl(int controlid, int id, const GuiPoint& location, GuiWnd* parent)
{
   GuiRegisteredControl* pcontrol = findControl(controlid);
   if ( pcontrol != NULL )
      return pcontrol->pfunction(id, location, parent);

   return NULL;
}

GuiRegisteredControl* GuiDesigner::findControl(int id)
{
   for ( int index = 0; index < registeredControls().size(); ++index )
   {
      GuiRegisteredControl& control = registeredControls()[index];
      if ( control._id == id )
         return &control;
   }

   return NULL;
}

int GuiDesigner::registerControl(int id, const std::string& name, const std::string& desc, createControlFnc pfunction)
{
   registeredControls().push_back(GuiRegisteredControl(id, name, desc, pfunction));
   return id;
}

GuiDesigner::GuiRegisteredControls& GuiDesigner::registeredControls()
{
   if ( _registeredControls == NULL )
      _registeredControls = new GuiRegisteredControls();

   ASSERT_PTR(_registeredControls)
   return *_registeredControls;
}

std::string GuiDesigner::controlName(GuiWnd* pcontrol)
{
   GuiRegisteredControl* pcontroldesc = findControl(pcontrol->getTypeId());
   if ( pcontroldesc != NULL )
   {
      return pcontroldesc->name;
   }

   return "";
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

GuiDesigner::GuiDesigner():
   GuiWindow(),
   _pprojectWorkspace(NULL),
   _ppropertyDlg(NULL),
   _peventhandlerdlg(NULL),
   _pdesignerPopup(NULL),
   _pwindowPopup(NULL),
   _popupMenu(NULL)
{
}

GuiDesigner::~GuiDesigner()
{
}

void GuiDesigner::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   int width, height;
   Game::getInstance().getWindowDimensions(width, height);
   GuiRect fullwindow(0, width, 0, height);

   GuiWindow::onCreate(fullwindow, caption, style, parent);

   createDesignerPopup();
   createWindowPopup();
   createProjectWorkspace();
   createPropertyWnd();
   createEventHandlerWnd();
   createWindow();
}

void GuiDesigner::createDesignerPopup()
{
   _pdesignerPopup = new GuiMenu();
   _pdesignerPopup->create(24, GuiRect(), "DesignerPopup", GUI_VISIBLE, NULL);
   _pdesignerPopup->setFont(getFont());

   _pdesignerPopup->addItem(200, "New Window");
   _pdesignerPopup->addItem(201, "New Menu");
   _pdesignerPopup->addItem(202, "Open");
   _pdesignerPopup->addItem(203, "Save All");
   _pdesignerPopup->addItem(204, "Close All");
   _pdesignerPopup->addItem(205, "Exit");

   _pdesignerPopup->calculateSize();
}

void GuiDesigner::createWindowPopup()
{
   _pwindowPopup = new GuiMenu();
   _pwindowPopup->create(21, GuiRect(0,0,0,0), "Menu", GUI_VISIBLE, NULL);
   _pwindowPopup->setFont(getFont());

   for ( Uint32 idx = 0; idx < registeredControls().size(); ++idx )
   {
      GuiRegisteredControl& control = registeredControls()[idx];
      _pwindowPopup->addItem(control._id, control._description);
   }

   _pwindowPopup->addItem(50, "Save");
   _pwindowPopup->addItem(51, "Close");

   _pwindowPopup->calculateSize();
}

void GuiDesigner::createProjectWorkspace()
{
   _pprojectWorkspace = static_cast<GuiDialog*>(GuiManager::getInstance().loadFromXML("des_projectworkspace"));
   _pprojectWorkspace->setParent(this);
   _pprojectWorkspace->setWindowPos(20, 30);
}

void GuiDesigner::createPropertyWnd()
{
   _ppropertyDlg = new GuiDialog();
   _ppropertyDlg->create(22, GuiRect(500, 700, 25, 275), "Properties", 392 | GUI_TOPMOST, this);
   _ppropertyDlg->setWindowPos(580, 30);

   GuiPropertyList* plist = new GuiPropertyList();
   plist->create(1, GuiRect(5, 195, 25, 220), "", 392 | GUI_LIST_HEADER, _ppropertyDlg);

   int col1idx = plist->header().addColumn(100);
   int col2idx = plist->header().addColumn(100);

   plist->header()[col1idx].title("Attributes");
   plist->header()[col2idx].title("Values");

   plist->setEditableColumn(1);
}

void GuiDesigner::createEventHandlerWnd()
{
   _peventhandlerdlg = GuiManager::getInstance().loadDialogFromXML("des_eventhandlers");
   _peventhandlerdlg->setTopmost(true);
   _peventhandlerdlg->setParent(this);
   _peventhandlerdlg->setWindowPos(580, 350);
}

GuiDesignWnd* GuiDesigner::createWindow()
{
   GuiDesignWnd* pwnd = new GuiDesignWnd();
   pwnd->create(1, GuiRect(20,300,20,300), "Window1", 392, this);
   pwnd->center();
   return pwnd;
}

GuiDesignMenu* GuiDesigner::createMenu()
{
   GuiDesignMenu* pmenu = new GuiDesignMenu();
   pmenu->create(1, GuiRect(20, 40, 20, 40), "Menu", GUI_VISIBLE, this);
   return pmenu;
}

//////////////////////////////////////////////////////////////////////////
// - Input interface
//////////////////////////////////////////////////////////////////////////

bool GuiDesigner::onContextMenu(const GuiPoint& point)
{
   const GuiRect& rect = getDesignerPopup().getWindowRect();

   if ( _popupMenu == NULL )
   {
      _popupMenu = new GuiPopupDecorator(getDesignerPopup());
      _popupMenu->create(100, GuiRect(point.x, point.x + rect.getWidth(), point.y, point.y + rect.getHeight()), "", GUI_VISIBLE, NULL);
      _popupMenu->setOwner(*this);

      getDesignerPopup().setParent(_popupMenu);
   }

   _popupMenu->popup();

   return true;
}

//////////////////////////////////////////////////////////////////////////
// - Context menu
//////////////////////////////////////////////////////////////////////////

void GuiDesigner::onMenuLostFocus()
{
   _popupMenu->hide();
}

void GuiDesigner::onCommand(int id)
{
   onMenuLostFocus();

   switch ( id )
   {
   case 200:
      createWindow();
      break;
   case 201:
      createMenu();
      break;
   case 202:
      load();
      break;
   }
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiDesigner::focusChanged(GuiWnd& wnd)
{
   GuiPropertyList* ppropertylist = dynamic_cast<GuiPropertyList*>(_ppropertyDlg->getItemById(1));
   ppropertylist->Properties(wnd.getProperties());
   ppropertylist->window(&wnd);

   GuiWnd* pwindow = dynamic_cast<GuiDesignWnd*>(&wnd);
   if ( pwindow == NULL )
   {
      pwindow = dynamic_cast<GuiDesignWnd*>(wnd.getBoundaryParent());

      if ( pwindow == NULL )
      {
         pwindow = dynamic_cast<GuiDesignMenu*>(&wnd);
      }
   }

   ASSERT_PTR(pwindow);

   {
      // move the window to the front
      GuiList::Iterator it = childs.find(pwindow);
      childs.removeAt(it);

      it = childs.begin();
      while ( it.valid() && (*it)->isTopmost() )
         ++it;

      childs.insert(it, pwindow);
   }

   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.prepareCall("designer_eh_setwindow");
   script.addParam(&wnd, "GuiWnd");
   script.run(1);
}

void GuiDesigner::load()
{
   GuiDialog* pdialog = GuiManager::getInstance().loadDialogFromXML("openfile");
   pdialog->center();
   if ( pdialog->doModal() == 0 )
   {
      GuiEditBox* pedit  = dynamic_cast<GuiEditBox*>(pdialog->findChildByName("edtFile"));

      if ( GuiManager::getInstance().isDialog(pedit->getCaption()) )
      {
         const std::string& scriptpath = Game::getInstance().getConfiguration().getScriptPath();
         std::string file = scriptpath + pedit->getCaption() + ".xml";
         GuiDesignWnd* pwindow = createWindow();
         pwindow->load(file);
      }
      else
      {
         ASSERT(GuiManager::getInstance().isMenu(pedit->getCaption()))
         GuiDesignMenu* pmenu = createMenu();
         pmenu->load(pedit->getCaption());
      }
   }
}

void GuiDesigner::close(GuiWnd* pwindow)
{
   setFocus();

   removeChild(pwindow);

   pwindow->destroy();
   delete pwindow;
}
