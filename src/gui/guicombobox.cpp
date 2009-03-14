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

#include "../script.h"
#include "../scriptmanager.h"

#include "guicanvas.h"
#include "guiclipper.h"
#include "guifont.h"
#include "guimanager.h"
#include "guidesigner.h"
#include "guipopupdecorator.h"
#include "guifocus.h"
#include "guitext.h"

EVENT_MAP_BEGIN(GuiComboBox)
   ON_LIST_SELCHANGE(1,onSelChange)
   ON_LOST_FOCUS(1, onListLostFocus)
   ON_BTN_CLICK(2,onDown)
EVENT_MAP_END()

REGISTER_DESIGNER(GuiComboBox, GuiComboBoxId, "Combobox", 40, 15, 392)

GuiComboBox::GuiComboBox():
   options(0),
   drop(0),
   _list(NULL),
   _listvisible(false)
{
}

GuiComboBox::GuiComboBox(int id, const GuiRect& rect, const char* caption, int style, GuiWnd* parent):
   options(0),
   drop(0)
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
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiComboBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   graphics.setColor(GuiManager::getInstance().getDefaultTextColor());

   int posX = m_frameRect.left() + 5;
   int posY = m_frameRect.top() + font->getBaseHeight();
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
// - Input interface
//////////////////////////////////////////////////////////////////////////

void GuiComboBox::onKeyDown (int which, bool shift, bool ctrl, bool alt)
{
   switch ( which )
   {
   case SDLK_UP:   options->setCurSel(options->getCurSel() - 1); break;
   case SDLK_DOWN: options->setCurSel(options->getCurSel() + 1); break;
   }
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

/***************************************************************
 * GuiColorComboBox
 */

GuiColorComboBox::GuiColorComboBox():
   GuiComboBox()
{
}

void GuiColorComboBox::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiComboBox::onCreate(rect, caption, style, parent);

   // remove the current options box from the window
   GuiRect client(options->getWindowRect());
   //delete options;
   delete _list;

   // create our own options listbox
   options = new GuiColorListBox();

   _list = new GuiPopupDecorator(*options);
   _list->create(3, client, "", GUI_VISIBLE, NULL);
   _list->setOwner(*this);

   options->create(1, client, "", GUI_BACKGROUND|GUI_BORDER|GUI_VISIBLE|GUI_NOTIFYPARENT, _list);
}

void GuiColorComboBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   GuiComboBox::paint(tick, graphics);

   int pLX = m_frameRect.left() + 5;
   int pUY = m_frameRect.top() + 5;
   int pRX = pLX + m_frameRect.getWidth() - 44;
   int pBY = pUY + m_frameRect.getHeight() - 10;

   GuiColorListBox* clb = static_cast<GuiColorListBox*>(options);

   graphics.setColor(clb->getColor(clb->getCurSel()));
   graphics.drawRect(GuiRect(pLX, pRX, pUY, pBY));
}

void GuiColorComboBox::GuiColorListBox::addColor(GuiColor color)
{
   colors.push_back(color);
   addString(" ");
}

void GuiColorComboBox::GuiColorListBox::drawItem(int idx, int posX, int posY, const GuiGraphics& graphics)
{
   // calculate the corner coordinates
   int pLX = posX + 5;
   int pRX = pLX + (m_frameRect.getWidth() - 16 - 10);
   int pUY = posY + 2;
   int pBY = pUY + font->getHeight() - 4;

   graphics.setColor(colors[idx]);
   graphics.drawRect(GuiRect(pLX, pRX, pUY, pBY));

   drawSelection(idx, posX, posY);
}
