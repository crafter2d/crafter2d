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
#include "guimanager.h"
#ifndef JENGINE_INLINE
#  include "guimanager.inl"
#endif

#include <fstream>
#include <ctype.h>
#include <tinyxml.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/autoptr.h"
#include "core/script/script.h"
#include "core/script/scriptmanager.h"
#include "core/vfs/unzipfile.h"

#include "engine/console.h"

#include "gui/guidialog/guidialog.h"
#include "gui/button/guibutton.h"
#include "gui/edit/guiedit.h"

#include "guitext.h"
#include "guidesigner.h"
#include "guifont.h"
#include "guistatic.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinitions.h"
#include "guipopupdecorator.h"
#include "guimenu.h"

GuiManager::GuiManager():
   mFreeTypeLib(NULL),
   mTextColor(),
   mpFont(NULL),
   mpPopup(NULL)
{
}

GuiManager::~GuiManager()
{
   destroy();
}

GuiManager& GuiManager::getInstance ()
{
	static GuiManager man;
	return man;
}

bool GuiManager::initialize ()
{
   if ( FT_Init_FreeType(&mFreeTypeLib) != 0 )
      return false;

	return true;
}

void GuiManager::destroy()
{
   if ( mpFont != NULL )
   {
      delete mpFont;
      mpFont = NULL;
   }

   if ( mFreeTypeLib != NULL )
   {
      FT_Done_FreeType(mFreeTypeLib);
      mFreeTypeLib = NULL;
   }
}

std::string GuiManager::constructPath(const std::string& filename)
{
   std::string scriptpath = Game::getInstance().getConfiguration().getScriptPath();
   return scriptpath + filename + ".xml";
}

bool GuiManager::isDialog(const std::string& name)
{
   std::string file = constructPath(name);
   TiXmlDocument doc;

   if ( !doc.LoadFile(file) )
      Console::getInstance().printf("Can not load contents of %s.", file.c_str());
   else
   {
      TiXmlElement* pdialog = (TiXmlElement*)doc.FirstChild("dialog");
      if ( pdialog != NULL )
         return true;
   }

   return false;
}

bool GuiManager::isMenu(const std::string& name)
{
   std::string file = constructPath(name);
   TiXmlDocument doc;

   if ( !doc.LoadFile(file) )
      Console::getInstance().printf("Can not load contents of %s.", file.c_str());
   else
   {
      TiXmlElement* pmenu = (TiXmlElement*)doc.FirstChild("menu");
      if ( pmenu != NULL )
         return true;
   }

   return false;
}

GuiDialog* GuiManager::loadDialogFromXML(const std::string& name)
{
   return dynamic_cast<GuiDialog*>(loadFromXML(name));
}

GuiMenu* GuiManager::loadMenuFromXML(const std::string& name)
{
   return dynamic_cast<GuiMenu*>(loadFromXML(name));
}

GuiWnd* GuiManager::loadFromXML(const std::string& name)
{
   std::string file = constructPath(name);
   TiXmlDocument doc;

   if ( !doc.LoadFile(file) )
   {
      Console::getInstance().printf("Can not load window from %s.", file.c_str());
      return NULL;
   }
   else
   {
      TiXmlElement* pdialog = (TiXmlElement*)doc.FirstChild("dialog");
      if ( pdialog != NULL )
         return loadDialogFromXML(*pdialog);

      TiXmlElement* pmenu = (TiXmlElement*)doc.FirstChild("menu");
      if ( pmenu != NULL )
         return loadMenuFromXML(*pmenu);

      Console::getInstance().printf("%s contains no known GUI item.", file.c_str());
      return NULL;
   }
}

GuiDialog* GuiManager::loadDialogFromXML(TiXmlElement& dialog)
{
   AutoPtr<GuiDialog> pdialog = new GuiDialog();

   int type, x, y, width, height;
   TiXmlElement* pself = (TiXmlElement*)dialog.FirstChild("self");

   pself->QueryIntAttribute("x", &x);
   pself->QueryIntAttribute("y", &y);
   pself->QueryIntAttribute("width", &width);
   pself->QueryIntAttribute("height", &height);

   GuiRect clientrect(x, x + width, y, y + height);
   pdialog->create(0, clientrect, "", GUI_BACKGROUND|GUI_BORDER|GUI_VISIBLE, NULL);
   pdialog->getEventHandlerDefinitions().fromXML(pself);
   pdialog->getEventHandlerDefinitions().registerAll(*pdialog);
   pdialog->getProperties().fromXML(pself);

   TiXmlElement* pchildren = (TiXmlElement*)dialog.FirstChild("children");
   TiXmlElement* pchild = (TiXmlElement*)pchildren->FirstChild("child");
   while ( pchild != NULL )
   {
      pchild->QueryIntAttribute("type", &type);
      pchild->QueryIntAttribute("x", &x);
      pchild->QueryIntAttribute("y", &y);
      pchild->QueryIntAttribute("width", &width);
      pchild->QueryIntAttribute("height", &height);

      GuiPoint p(x,y);
      GuiControl* pcontrol = GuiDesigner::createControl(type, 0, p, pdialog.getPointer());
      ASSERT_PTR(pcontrol)

      pcontrol->getProperties().fromXML(pchild);
      pcontrol->getEventHandlerDefinitions().fromXML(pchild);
      pcontrol->getEventHandlerDefinitions().registerAll(*pcontrol);

      pcontrol->resizeWindow(width, height);

      pchild = (TiXmlElement*)pchildren->IterateChildren("child", pchild);
   }

   TiXmlElement* pfunctions = (TiXmlElement*)dialog.FirstChild("functions");
   if ( pfunctions != NULL )
   {
      TiXmlElement* pfunction = (TiXmlElement*)pfunctions->FirstChild("function");
      while ( pfunction != NULL )
      {
         TiXmlText* pcode = (TiXmlText*)pfunction->FirstChild();
         ScriptManager::getInstance().executeLine(pcode->Value());

         pfunction = (TiXmlElement*)pfunctions->IterateChildren("function", pfunction);
      }
   }

   GuiEventHandler* phandler = pdialog->getEventHandlers().findByEventType(GuiDialogLoadedEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(pdialog.getPointer(), "GuiDialog");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.run(0);
   }

   return pdialog.release();
}

GuiDialog* GuiManager::loadSystemDialog(const std::string& name)
{
   int length;
   char* pdata;

   std::string path = "ui/" + name + ".xml";

   UnzipFile file("data.zip");
   if ( file.readFile(path, (void*&)pdata, length) )
   {
      TiXmlDocument doc;

      std::stringstream stream;
      stream << pdata;
      stream >> doc;

      delete pdata;
      pdata = NULL;

      TiXmlElement* pdialog = (TiXmlElement*)doc.FirstChild("dialog");
      if ( pdialog != NULL )
         return loadDialogFromXML(*pdialog);
   }

   return NULL;
}

GuiMenu* GuiManager::loadMenuFromXML(TiXmlElement& menu)
{
   int id = 0;

   menu.QueryIntAttribute("id", &id);
   std::string name = menu.Attribute("name");

   AutoPtr<GuiMenu> pmenu = new GuiMenu();
   ASSERT(pmenu.hasPointer())

   pmenu->create(id, GuiRect(0,0,0,0), "Menu", GUI_VISIBLE, NULL);
   pmenu->setName(name);
   pmenu->getEventHandlerDefinitions().fromXML(&menu);
   pmenu->getEventHandlerDefinitions().registerAll(*pmenu);

   // load the menu items
   TiXmlElement* pitems = (TiXmlElement*)menu.FirstChild("menuitems");
   TiXmlElement* pitem  = (TiXmlElement*)pitems->FirstChild("item");
   while ( pitem != NULL )
   {
      int itemid = 0;

      pitem->QueryIntAttribute("id", &itemid);
      std::string text = pitem->Attribute("text");
      pmenu->addItem(itemid, text);

      pitem = (TiXmlElement*)pitems->IterateChildren("item", pitem);
   }

   pmenu->calculateSize();
   return pmenu.release();
}

void GuiManager::showPopup(GuiWnd& owner, const Point& point, const std::string& name)
{
   GuiMenu* pmenu = dynamic_cast<GuiMenu*>(loadFromXML(name));
   if ( pmenu != NULL )
   {
      const GuiRect& rect = pmenu->getWindowRect();

      if ( mpPopup == NULL )
      {
         mpPopup = new GuiPopupDecorator();
         mpPopup->create(100, GuiRect(0, 0, 0, 0), "", GUI_VISIBLE, NULL);
      }

      Point localpoint = point;
      owner.clientToWindow(localpoint);

      mpPopup->setWindowPos(localpoint.x, localpoint.y);
      mpPopup->resizeWindow(rect.getWidth(), rect.getHeight());
      mpPopup->setPopup(*pmenu, true);
      mpPopup->setOwner(owner);

      pmenu->setLocation(point);
      pmenu->setParent(mpPopup);

      mpPopup->popup();
   }
}
