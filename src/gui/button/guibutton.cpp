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

#include "script.h"
#include "scriptmanager.h"

#include "gui/guigraphics.h"
#include "gui/guimanager.h"
#include "gui/guidesigner.h"
#include "gui/guifont.h"
#include "gui/guieventhandler.h"
#include "gui/guieventhandlers.h"
#include "gui/guieventhandlerdefinition.h"
#include "gui/guieventhandlerdefinitions.h"
#include "gui/guitext.h"

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
   mTextPos(),
   mMouseListener(*this),
   mIcon(),
   mIconRect(),
   mCommand()
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

   addMouseListener(mMouseListener);

   // make sure we are not pressed
   CLEAR_FLAG(m_style, GUI_PRESSED);
}

//-----------------------------------
// - Overloads
//-----------------------------------

void GuiButton::onResize(int width, int height)
{
   // calculate the position for the text
   const GuiRect& rect = getWindowRect();
   int len = getCaption().length();
   int x   = rect.left() + (width / 2);
   int y   = rect.top() + (height / 2);

   if ( len == 0 )
   {
      mIconRect.set(x - 8, x + 8, y - 8, y + 8);
   }
   else
   {
      Vector size = font->getTextSize(getCaption());
      mTextPos.x = x - (size.x / 2);
      mTextPos.y = y + (size.y / 2);

      mIconRect.set(mTextPos.x - 20, mTextPos.x - 4, y - 8, y + 8);
   }
}

void GuiButton::onKillFocus(GuiControl* newCtrl)
{
   GuiControl::onKillFocus(newCtrl);
   if ( isPressed() )
   {
      m_style ^= GUI_PRESSED;
   }
}

/// \fn GuiButton::paint(Uint32 tick)
/// \brief Draws the button.
void GuiButton::paint(Uint32 tick, const GuiGraphics& graphics)
{
   if ( getBackground() )
   {
      if ( isPressed() )
      {
         graphics.drawSunkenRect(m_frameRect);
      }
      else
      {
         if ( getDefault() )
            graphics.drawDefaultRect(m_frameRect);
         else
            graphics.drawRaisedRect(m_frameRect);

         if ( isHoovering() )
         {
            GuiRect rect(m_frameRect.left()+2, m_frameRect.right()-2, m_frameRect.top()+2, m_frameRect.bottom()-2);
            graphics.setColor(GuiColor(213, 150, 2));
            graphics.drawWireRect(rect);
         }
      }
   }

   if ( mIcon.valid() )
   {
      graphics.setColor(1,1,1);
      graphics.drawImage(*mIcon, mIconRect);
   }

   // draw the text
   if ( !getCaption().empty() )
   {
      GuiFont* font = parent->getFont ();
      GuiColor color(GuiManager::getInstance().getDefaultTextColor());
      glColor3fv(&color.r);
      GuiText::printfn(*font, mTextPos.x, mTextPos.y, getCaption());
   }
}

//-----------------------------------
// - Operations
//-----------------------------------

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
   mCommand = cmd;
}

void GuiButton::pressed(bool press)
{
   if ( press )
      SET_FLAG(m_style, GUI_PRESSED);
   else
      CLEAR_FLAG(m_style, GUI_PRESSED);
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
   else if (!mCommand.empty())
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      mgr.setObject(this, "GuiButton", "self");
      mgr.executeLine(mCommand.c_str());
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
      mIcon = ResourceManager::getInstance().loadTexture(icon.c_str());
   }
}
