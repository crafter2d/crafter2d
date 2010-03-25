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

#include "guidesignmenu.h"
#ifndef JENGINE_INLINE
#  include "guidesignmenu.inl"
#endif

#include <sstream>
#include <tinyxml.h>

#include "game.h"

#include "gui/guidialog/guidialog.h"

#include "guiedit.h"
#include "guimanager.h"
#include "guidesigner.h"
#include "guieventhandlerdefinitions.h"

GuiDesignMenu::GuiDesignMenu():
   GuiMenu(),
   _filename(),
   _movingItem(false)
{
}

//////////////////////////////////////////////////////////////////////////
// - Input interface
//////////////////////////////////////////////////////////////////////////

int GuiDesignMenu::onLButtonUp(const GuiPoint& point, int flag)
{
   return JENGINE_MSG_HANDLED;
}

void GuiDesignMenu::onKeyUp(int which)
{
   SDLMod mode = SDL_GetModState();
   if ( (mode & KMOD_CTRL) != 0 )
   {
      if ( which == SDLK_UP )
         moveSelectionUp();
      else if ( which == SDLK_DOWN )
         moveSelectionDown();
   }
}

bool GuiDesignMenu::onContextMenu(const GuiPoint& point)
{
   GuiManager::getInstance().showPopup(*this, point, "des_menudesign_popup");
   return true;
}

void GuiDesignMenu::onCommand(int cmd)
{
   switch ( cmd )
   {
   case 0:
      addItem("item");
      break;
   case 1:
      editItem(getSelection());
      break;
   case 2:
      deleteItem(getSelection());
      break;
   case 3:
      save();
      break;
   case 4:
      close();
      break;
   }
}

void GuiDesignMenu::onSetFocus(GuiWnd* oldCtrl)
{
   GuiMenu::onSetFocus(oldCtrl);

   Game::getInstance().getCanvas().getDesigner().focusChanged(*this);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiDesignMenu::addItem(const std::string& item)
{
   int id = getItems().size();

   GuiMenu::addItem(id, item);

   calculateSize();
}

void GuiDesignMenu::editItem(int itemindex)
{
   if ( itemindex >= 0 && itemindex < getItems().size() )
   {
      GuiMenuItem& item = getItems()[itemindex];

      GuiDialog* pdialog = GuiManager::getInstance().loadDialogFromXML("des_menu_edit_item");

      std::stringstream stream;
      stream << item.getId();

      GuiWnd* pid = pdialog->findChildByName("designer_mi_id");
      pid->setCaption(stream.str());

      GuiWnd* ptext = pdialog->findChildByName("designer_mi_text");
      ptext->setCaption(item.text());

      if ( pdialog->doModal() == 0 )
      {
         item.setId(atoi(pid->getCaption().c_str()));
         item.text(ptext->getCaption());
      }
   }
}

void GuiDesignMenu::deleteItem(int item)
{
   if ( getSelection() >= 0 )
   {
      GuiMenuItems::iterator it = getItems().begin();
      for ( int i = 0; i < item; ++item, ++it ) {};

      getItems().erase(it);
   }
}

void GuiDesignMenu::close()
{
   GuiDesigner* pdesigner = dynamic_cast<GuiDesigner*>(getParent());
   ASSERT_PTR(pdesigner)

   pdesigner->close(this);
}

void GuiDesignMenu::moveSelectionUp()
{
   if ( getSelection() > 0 )
   {
      GuiMenuItem first = getItems()[getSelection() - 1];
      getItems()[getSelection() - 1] = getItems()[getSelection()];
      getItems()[getSelection()] = first;

      setSelection(getSelection() - 1);
   }
}

void GuiDesignMenu::moveSelectionDown()
{
   if ( getSelection() < getItems().size() - 1 )
   {
      GuiMenuItem first = getItems()[getSelection() + 1];
      getItems()[getSelection() + 1] = getItems()[getSelection()];
      getItems()[getSelection()] = first;

      setSelection(getSelection() + 1);
   }
}

bool GuiDesignMenu::load(const std::string& name)
{
   _filename = "../scripts/" + name + ".xml";
   TiXmlDocument doc;

   if ( doc.LoadFile(_filename) )
   {
      int id = 0;
      TiXmlElement* pmenu = (TiXmlElement*)doc.FirstChild("menu");
      pmenu->QueryIntAttribute("id", &id);
      std::string name = pmenu->Attribute("name");

      setName(name);
      getEventHandlerDefinitions().fromXML(pmenu);
      getEventHandlerDefinitions().registerAll(*this);

      // load the menu items
      TiXmlElement* pitems = (TiXmlElement*)pmenu->FirstChild("menuitems");
      TiXmlElement* pitem  = (TiXmlElement*)pitems->FirstChild("item");
      while ( pitem != NULL )
      {
         int itemid = 0;

         pitem->QueryIntAttribute("id", &itemid);
         std::string text = pitem->Attribute("text");
         GuiMenu::addItem(itemid, text);

         pitem = (TiXmlElement*)pitems->IterateChildren("item", pitem);
      }

      calculateSize();

      return true;
   }

   return false;
}

void GuiDesignMenu::save()
{
   if ( _filename.empty() )
   {
      GuiDialog* pdialog = GuiManager::getInstance().loadDialogFromXML("openfile");
      pdialog->center();

      if ( pdialog->doModal() == 0 )
      {
         GuiEditBox* pedit  = dynamic_cast<GuiEditBox*>(pdialog->findChildByName("edtFile"));
         _filename = "../scripts/" + pedit->getCaption() + ".xml";
      }
      else
      {
         return;
      }
   }

   doSave();
}

void GuiDesignMenu::doSave()
{
   TiXmlElement* pmenu      = new TiXmlElement("menu");
   TiXmlElement* pmenuitems = new TiXmlElement("menuitems");

   for ( int index = 0; index < getItems().size(); ++index )
   {
      const GuiMenuItem& item = getItems()[index];
      std::stringstream stream;
      stream << item.getId();

      TiXmlElement* pitem = new TiXmlElement("item");
      pitem->SetAttribute("id", stream.str());
      pitem->SetAttribute("text", item.text());
      
      pmenuitems->LinkEndChild(pitem);
   }

   pmenu->SetAttribute("id", getId());
   pmenu->SetAttribute("name", getName());
   pmenu->LinkEndChild(getEventHandlerDefinitions().toXML());
   pmenu->LinkEndChild(pmenuitems);

   TiXmlDocument doc;
   doc.LinkEndChild(new TiXmlDeclaration("1.0", "", ""));
   doc.LinkEndChild(pmenu);
   doc.SaveFile(_filename);
}
