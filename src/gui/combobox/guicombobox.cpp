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
#include "guicombobox.h"
#ifndef JENGINE_INLINE
#  include "guicombobox.inl"
#endif

#include "script.h"
#include "scriptmanager.h"

#include "gui/button/guibutton.h"

#include "gui/guiclipper.h"
#include "gui/guifont.h"
#include "gui/guimanager.h"
#include "gui/guidesigner.h"
#include "gui/guilistbox.h"
#include "gui/guipopupdecorator.h"
#include "gui/guifocus.h"
#include "gui/guitext.h"

EVENT_MAP_BEGIN(GuiComboBox)
   ON_LIST_SELCHANGE(1,onSelChange)
   ON_LOST_FOCUS(1, onListLostFocus)
   ON_BTN_CLICK(2,onDown)
EVENT_MAP_END()

REGISTER_DESIGNER(GuiComboBox, GuiComboBoxId, "Combobox", 40, 15, 392)

GuiComboBox::GuiComboBox():
   GuiControl(),
   mKeyListener(*this),
   options(0),
   drop(0),
   _list(NULL),
   _listvisible(false),
   callbackFnc(),
   selectionbox()
{
}

GuiComboBox::GuiComboBox(int id, const GuiRect& rect, const char* caption, int style, GuiWnd* parent):
   GuiControl(),
   mKeyListener(*this),
   options(0),
   drop(0),
   _list(NULL),
   _listvisible(false),
   callbackFnc(),
   selectionbox()
{
   create(id, rect, caption, style, parent);
}

void GuiComboBox::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   GuiRect client(0, rect.getWidth(), 0, 100);

   // create the options listbox
   options = new GuiListBox();

   _list = new GuiPopupDecorator(*options);
   _list->create(3, client, "", GUI_VISIBLE, NULL);
   _list->setOwner(*this);

   options->create(1, client, "", GUI_BACKGROUND|GUI_BORDER|GUI_VISIBLE|GUI_NOTIFYPARENT, _list);

   // create the drop down button
   drop = new GuiButton();
   drop->create(2, GuiRect(rect.getWidth()-34, rect.getWidth(), 0, rect.getHeight()), "v", GUI_BACKGROUND|GUI_BORDER|GUI_VISIBLE, this);

   addKeyListener(mKeyListener);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiComboBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   graphics.setColor(GuiManager::getInstance().getDefaultTextColor());

   float posX = m_frameRect.left() + 5;
   float posY = m_frameRect.top() + font->getBaseHeight();
   GuiText::printfn(*font, posX, posY, getCaption());
}

//////////////////////////////////////////////////////////////////////////
// - Events
//////////////////////////////////////////////////////////////////////////

void GuiComboBox::onResize(int width, int height)
{
   const GuiRect& rect = getWindowRect();
   drop->setWindowRect(GuiRect(rect.getWidth()-34, rect.getWidth(), 0, rect.getHeight()));

   GuiRect client(0, rect.getWidth(), 0, 100);
   options->setWindowRect(client);

   _list->resizeWindow(client.getWidth(), 100);
}

void GuiComboBox::onKillFocus(GuiWnd* newCtrl)
{
   if ( !isChild(newCtrl) && newCtrl != options )
   {
      displayList(false);

      GuiControl::onKillFocus(newCtrl);
   }
}

void GuiComboBox::onDown()
{
   // make sure we are current, not the button
   displayList(!_listvisible);
}

void GuiComboBox::onSelChange(int newSel)
{
   const char* text = options->getItemText(newSel);
   setCaption(text);

   displayList(false);
   setFocus();

   if (!callbackFnc.empty())
   {
      // notify the game about this change
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiComboBox");
      script.prepareCall(callbackFnc.c_str());
      script.addParam(1);
      script.run(1);
   }
}

void GuiComboBox::onListLostFocus()
{
   onKillFocus(NULL);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiComboBox::displayList(bool display)
{
   if ( display != _listvisible )
   {
      if ( display )
      {
         GuiRect windowrect(getWindowRect());
         clientToWindow(windowrect);

         _list->setWindowPos(windowrect.left(), windowrect.bottom()+5);
         _list->popup();
      }
      else
      {
         _list->hide();
      }

      _listvisible = display;
   }
}

void GuiComboBox::removeAll()
{
   options->removeAll();
}

int GuiComboBox::addString(const char* str)
{
   return options->addString(str);
}
