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
#include "guibutton.h"
#ifndef JENGINE_INLINE
#  include "guibutton.inl"
#endif

#include <SDL/SDL.h>
#include "../script.h"
#include "../scriptmanager.h"

#include "guigraphics.h"
#include "guimanager.h"
#include "guidesigner.h"
#include "guifont.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"
#include "guitext.h"

#define CLICK_TIMEINTERVAL  800

REGISTER_DESIGNER(GuiButton, GuiButtonId, "Button", 40, 15, 264)

IMPLEMENT_PROPERTIES(GuiButton, GuiControl)
   CONSTRUCT_BOOL_PROPERTY(Default, "Default", false)
   CONSTRUCT_STR_PROPERTY_WITH_CALLBACK(Icon, "Icon", "", onIconChanged)
}

void GuiButton::onIconChanged(GuiWnd* pwnd)
{
   GuiButton* pbutton = dynamic_cast<GuiButton*>(pwnd);
   pbutton->loadIcon(pbutton->getIcon());
}

void GuiButton::initializeEventHandlerDefinitions()
{
   GuiControl::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiButtonClickEvent, "onClick");

   getEventHandlerDefinitions().add(pdefinition);
}

GuiButton::GuiButton():
   pos(),
   pressTime(0),
   command()
{
}

/// \fn GuiButton::create(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
/// \brief Initialize the button object
///
/// Initializes the button object and precalculates the text position so the caption appears in the center
/// of the button.
void GuiButton::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   // create the window
   GuiWnd::onCreate(rect, caption, style, parent);

   onResize(rect.getWidth(), rect.getHeight());

   // make sure we are not pressed
   m_style &= ~GUI_PRESSED;
}

void GuiButton::onResize(int width, int height)
{
   // calculate the position for the text
   const GuiRect& rect = getWindowRect();
   int len = getCaption().length();
   int x   = rect.left() + (width / 2);
   int y   = rect.top() + (height / 2);

   if ( len == 0 )
   {
      _iconRect.set(x - 8, x + 8, y - 8, y + 8);
   }
   else
   {
      Vector size = font->getTextSize(getCaption());
      pos.x = x - (size.x / 2);
      pos.y = y + (size.y / 2);

      _iconRect.set(pos.x - 20, pos.x - 4, y - 8, y + 8);
   }
}

/// \fn GuiButton::setCommand(const char* cmd)
/// \brief Sets the command to be executed when the user clicks on it.
/// \param cmd The command to be executed on a click
///
/// This function saves the command for this button. When it is clicked (received mousedown and -up
/// events within a certain time interval) this command is executed in the Lua environment.
///
/// See \ref guiWnd_lua for more information and an example implementation.
void GuiButton::setCommand (const char* cmd)
{
   command = cmd;
}

/// \fn GuiButton::paint(Uint32 tick)
/// \brief Draws the button.
void GuiButton::paint(Uint32 tick, const GuiGraphics& graphics)
{
   if ( getBackground() )
   {
      if ( IS_SET(m_style, GUI_PRESSED) )
         graphics.drawSunkenRect(m_frameRect);
      else if ( getDefault() )
         graphics.drawDefaultRect(m_frameRect);
      else
         graphics.drawRaisedRect(m_frameRect);
   }

   if ( _icon.valid() )
   {
      graphics.setColor(1,1,1);
      graphics.drawImage(*_icon, _iconRect);
   }

   // draw the text
   if ( !getCaption().empty() )
   {
      GuiFont* font = parent->getFont ();
      GuiColor color(GuiManager::getInstance().getDefaultTextColor());
      glColor3fv(&color.r);
      GuiText::printfn(*font, pos.x, pos.y, getCaption());
   }
}

int GuiButton::onLButtonDown (const GuiPoint& point, int flag)
{
   m_style |= GUI_PRESSED;
   pressTime = SDL_GetTicks();

   return JENGINE_MSG_HANDLED;
}

int GuiButton::onLButtonUp (const GuiPoint& point, int flag)
{
   m_style ^= GUI_PRESSED;
   Uint32 releaseTime = SDL_GetTicks();
   if (releaseTime - pressTime < CLICK_TIMEINTERVAL)
   {
      click();
   }

   return JENGINE_MSG_HANDLED;
}

void GuiButton::onKillFocus(GuiControl* newCtrl)
{
   GuiControl::onKillFocus(newCtrl);
   if ( isPressed() )
   {
      m_style ^= GUI_PRESSED;
   }
}

void GuiButton::click()
{
   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiButtonClickEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.prepareCall(phandler->getFunctionName().c_str());
      script.run(0);
   }
   else if (!command.empty())
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      mgr.setObject(this, "GuiButton", "self");
      mgr.executeLine(command.c_str());
   }
   else if (parent)
   {
      // notify the button click
      parent->sendMessage(m_id, GuiButtonClickEvent);
   }
}

void GuiButton::loadIcon(const std::string& icon)
{
   if ( !icon.empty() )
   {
      _icon = ResourceManager::getInstance().loadTexture(icon.c_str());
   }
}
