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
#include "../script.h"
#include "../defines.h"

#include "guiedit.h"
#include "guidesigner.h"
#include "guifont.h"
#include "guiscrollbar.h"
#include "guitext.h"

REGISTER_DESIGNER(GuiEditBox, GuiEditBoxId, "Editbox", 40, 15, 392)

EVENT_MAP_BEGIN(GuiEditBox)
   ON_SB_UP(1, onScroll)
   ON_SB_DOWN(1, onScroll)
   ON_SB_POSCHANGED(1, onScroll)
EVENT_MAP_END()

IMPLEMENT_PROPERTIES(GuiEditBox, GuiControl)
   getCaptionProperty().registerCallback((GuiPropertyBase::CallbackFnc)onCaptionChanged);

   CONSTRUCT_BOOL_PROPERTY_WITH_CALLBACK(MultiLine, "Multi line", false, onMultiLineChanged)
   CONSTRUCT_BOOL_PROPERTY_WITH_CALLBACK(WordWrap, "Wordwrap", false, (GuiPropertyBase::CallbackFnc)onWordWrapChanged)
}

void GuiEditBox::onCaptionChanged(GuiEditBox* pedit)
{
   pedit->handleCaptionChanged();
}

void GuiEditBox::onMultiLineChanged(GuiEditBox* pedit)
{
   pedit->handleMultiLineChanged();
}

void GuiEditBox::onWordWrapChanged(GuiEditBox* pedit)
{
   pedit->handleWordWrapChanged();
}

GuiEditBox::GuiEditBox():
   INIT_PROPERTY(MultiLine),
   INIT_PROPERTY(WordWrap),
   _pvertscrollbar(NULL),
   _lines(),
   maxChars(0),
   maxLines(0),
   scrollPos(0),
   scrollPosY(0),
   showCarret(true),
   carretPos(),
   lastTick(0)
{
}

void GuiEditBox::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
	GuiControl::onCreate(rect, caption, style, parent);

   onResize(getWindowRect().getWidth(), getWindowRect().getHeight());

   initializeScrollbar();
}

void GuiEditBox::initializeScrollbar()
{
   const GuiRect& rect = getWindowRect();
   int w = rect.getWidth();

   if ( _pvertscrollbar != NULL )
   {
      removeChild(_pvertscrollbar);
      delete _pvertscrollbar;
   }

   int flags = GUI_BACKGROUND;
   if ( getMultiLine() )
      flags |= GUI_VISIBLE;

   _pvertscrollbar = new GuiScrollBar();
   _pvertscrollbar->create(1, GuiRect(w-15, w, 1, rect.getHeight()-2), "", flags, this);
   _pvertscrollbar->setScrollInfo(0);

   setVertScrollBarRange();
}

void GuiEditBox::onResize(int width, int height)
{
   maxChars = getWindowRect().getWidth() / font->getAverageWidth();
   maxLines = getWindowRect().getHeight() / font->getHeight();

   initializeScrollbar();
}

void GuiEditBox::onScroll(int pos)
{
   scrollPosY = pos;
}

void GuiEditBox::setVertScrollBarRange()
{
   _pvertscrollbar->setScrollRange(MAX(getLineCount() - maxLines, 0));
}

void GuiEditBox::onSetFocus(GuiControl* oldCtrl)
{
   scrollPos = MIN(carretPos.x, MAX(getLineLength(carretPos.y) - maxChars, 0));

   GuiControl::onSetFocus(oldCtrl);
}

void GuiEditBox::handleCaptionChanged()
{
   carretPos.x = 0;
   carretPos.y = 0;

   scrollPos = 0;
   scrollPosY = 0;

   if ( getWordWrap() )
      handleWordWrapChanged();

   initializeScrollbar();
}

void GuiEditBox::handleMultiLineChanged()
{
   initializeScrollbar();
}

void GuiEditBox::handleWordWrapChanged()
{
   GuiFont* font           = parent->getFont ();
   const int maxlinelength = (getWindowRect().getWidth() - 18) / font->getAverageWidth();
   int pos                 = 0;

   _lines.clear();

   const std::string& caption = getCaption();
   while ( pos < getCaption().length() )
   {
      int newline = caption.find('\n', pos);
      int length  = ( newline != -1 && newline < pos + maxlinelength ) ? newline-pos : maxlinelength;
      length      = MIN(length, caption.length() - pos);

      if ( pos + length < caption.length() )
      {
         for ( int index = pos + length; index > pos; --index, --length )
         {
            if ( caption[index] == ' ' || caption[index] == '\n' )
               break;
         }
      }

      if ( length > 0 )
      {
         _lines.push_back(caption.substr(pos, length));

         pos += length + 1;
      }
      else
         break;
   }

   if ( pos < caption.length() )
   {
      _lines.push_back(caption.substr(pos));
   }
}

void GuiEditBox::ensureVisible()
{
   if ( getMultiLine() )
   {
      if ( carretPos.y < scrollPosY )
         _pvertscrollbar->setScrollPosition(carretPos.y);
      else
      {
         int maxVisible = getWindowRect().getHeight() / getParent()->getFont()->getHeight();
         if ( carretPos.y >= scrollPosY + maxVisible )
            _pvertscrollbar->setScrollPosition(carretPos.y - maxVisible + 1);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// - Painting
//////////////////////////////////////////////////////////////////////////

void GuiEditBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   glColor3f(1,0,0);
   if ( getWordWrap() )
   {
      GuiFont* font        = parent->getFont ();
      const int x          = m_frameRect.left() + 2;
            int y          = m_frameRect.top() + font->getBaseHeight();

      for ( int index = 0; index < _lines.size(); ++index )
      {
         GuiText::printfn(*font, x, y, _lines[index].c_str());

         y += font->getHeight();
      }
   }
   else
   {
      paintText();
   }

   if ( hasFocus() )
   {
      // render the carret
      if ( showCarret )
      {
         const GuiFont& font = *parent->getFont();
         int xpos = m_frameRect.left() + ((carretPos.x - scrollPos) * font.getAverageWidth()) + 3;
         int ypos = m_frameRect.top() + 2 + ((carretPos.y - scrollPosY) * font.getHeight());

         glBegin(GL_LINES);
         glVertex2i(xpos, ypos);
         glVertex2i(xpos, ypos + font.getHeight()-2);
         glEnd();
      }

      if ( tick - lastTick > 500 )
      {
         // swap the display of the carret
         lastTick = tick;
         showCarret = !showCarret;
      }
   }

   glColor3f(1,1,1);
}

void GuiEditBox::paintText()
{
   GuiFont* font        = parent->getFont ();
   const char* caption  = getCaption().c_str();
   const char* pnewline = strchr(caption, '\n');
   const char* pcurline = caption;

   int count = 0;
   while ( count < scrollPosY && pnewline != NULL )
   {
      pcurline = pnewline + 1;
      pnewline = strchr(pcurline, '\n');
      ++count;
   }

   const int x          = m_frameRect.left() + 2;
         int y          = m_frameRect.top() + font->getBaseHeight();

   while ( pnewline != NULL )
   {
      int length = pnewline - pcurline - scrollPos;
      GuiText::printfn(*font, x, y, &pcurline[scrollPos], length);

      y += font->getHeight();

      pcurline = pnewline + 1;
      pnewline = strchr(pcurline, '\n');
   }

   int length = strlen(pcurline) - scrollPos;
   GuiText::printfn(*font, x, y, &pcurline[scrollPos], length);
}

//////////////////////////////////////////////////////////////////////////
// - Input interface
//////////////////////////////////////////////////////////////////////////

int GuiEditBox::onLButtonDown(const GuiPoint& point, int flags)
{
   if ( GuiControl::onLButtonDown(point, flags) == JENGINE_MSG_UNHANDLED )
   {
      GuiPoint loc(point);

      GuiFont& font = *getParent()->getFont();
      carretPos.x = loc.x / font.getAverageWidth() + scrollPos;
      carretPos.y = loc.y / font.getHeight() + scrollPosY;

      if ( carretPos.y >= getLineCount() )
         carretPos.y = getLineCount() - 1;

      int linelength = getLineLength(carretPos.y);
      if ( carretPos.x >= linelength )
         carretPos.x = linelength;
   }

   return JENGINE_MSG_HANDLED;
}

void GuiEditBox::onKeyDown (int which, bool shift, bool ctrl, bool alt)
{
   lastTick = SDL_GetTicks();
   showCarret = true;

	switch (which) {
	case SDLK_BACKSPACE:
		doBackspace();
      break;
   case SDLK_DELETE:
      doDelete();
      break;
   case SDLK_HOME:
      carretPos.x = 0;
      scrollPos   = 0;
      break;
   case SDLK_END:
      carretPos.x = getLineLength(carretPos.y);
      scrollPos   = MAX(carretPos.x - maxChars, 0);
      break;
   case SDLK_LEFT:
      moveLeft(ctrl);
      break;
   case SDLK_RIGHT:
      moveRight(ctrl);
      break;
   case SDLK_UP:
      moveUp();
      break;
   case SDLK_DOWN:
      moveDown();
      break;
   case SDLK_RETURN:
      doReturn(shift, ctrl, alt);
      break;
   case SDLK_TAB:
      doTab();
      break;
	default:
      if ( which >= 0 && which < 256 )
         doInsert(which);
		break;
	}

   ensureVisible();
}

//////////////////////////////////////////////////////////////////////////
// - Queries
//////////////////////////////////////////////////////////////////////////

int GuiEditBox::getLineCount() const
{
   int count            = 1;
   const char* ptext    = getCaption().c_str();
   const char* pnewline = strchr(ptext, '\n');

   while ( pnewline != NULL )
   {
      ptext = pnewline + 1;
      pnewline = strchr(ptext, '\n');
      count++;
   }

   return count;
}

int GuiEditBox::getLineLength(int index) const
{
   int count            = 0;
   const char* ptext    = getCaption().c_str();
   const char* pnewline = strchr(ptext, '\n');

   while ( pnewline != NULL && count < index )
   {
      ptext = pnewline + 1;
      pnewline = strchr(ptext, '\n');
      count++;
   }

   if ( pnewline == NULL )
      return strlen(ptext);
   else
      return pnewline - ptext;
}

int GuiEditBox::carretToStringPos(bool useOffset) const
{
   int total            = 0;
   int count            = 0;
   int maxy             = carretPos.y + (useOffset ? scrollPosY : 0);
   const char* ptext    = getCaption().c_str();
   const char* pnewline = strchr(ptext, '\n');

   while ( pnewline != NULL && count < maxy )
   {
      total += (pnewline - ptext) + 1;

      ptext    = pnewline + 1;
      pnewline = strchr(ptext, '\n');
      count++;
   }

   return total + carretPos.x;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////


void GuiEditBox::doReturn(bool shift, bool ctrl, bool alt)
{
   if ( getMultiLine() )
   {
      static char insert[2] = { '\n' };
      int pos = carretToStringPos();
      getCaption().insert(pos, insert);

      scrollPos = 0;

      setVertScrollBarRange();

      carretPos.x = 0;
      carretPos.y++;
   }
   else
   {
      if ( hasParent() )
         getParent()->onKeyDown(SDLK_RETURN, shift, ctrl, alt);
   }
}

void GuiEditBox::doInsert(char character)
{
   if ( isprint(character) )
   {
      char buf[2] = { 0 };
      sprintf(buf, "%c", character);
      int pos = carretToStringPos();
      getCaption().insert(pos, buf);

      carretPos.x++;

      // if message to long, make it scroll automatically
      if ( carretPos.x > maxChars )
         ++scrollPos;
	}
}

void GuiEditBox::doBackspace()
{
   // carret pos is after the one we want to erase, thus -1 to correct for erase
   int pos = carretToStringPos() - 1;
   if ( pos >= 0 )
   {
      if ( getCaption()[pos] == '\n' )
      {
         carretPos.y--;
         carretPos.x = getLineLength(carretPos.y);

         setVertScrollBarRange();
      }
      else
         carretPos.x--;

      getCaption().erase(pos, 1);

      if ( carretPos.x > maxChars )
         --scrollPos;
      else
         scrollPos = 0;
   }
}

void GuiEditBox::doDelete()
{
   int pos = carretToStringPos();
   if ( getCaption()[pos] == '\n' )
      setVertScrollBarRange();

   getCaption().erase(pos, 1);
}

void GuiEditBox::doTab()
{
   getCaption().insert(carretToStringPos(), "   ");
   carretPos.x += 3;
}

void GuiEditBox::moveLeft(bool ctrl)
{
   if ( carretPos.x <= 0 && getMultiLine() && carretPos.y > 0 )
   {
      carretPos.y--;
      carretPos.x = getLineLength(carretPos.y) + 1;
   }

   if ( carretPos.x > 0 )
   {
      if ( ctrl )
      {
         const char* ptext = getCaption().c_str();
         skipWhitespace(ptext, false);
         skipToWhitespace(ptext, false);
      }
      else
         --carretPos.x;

      if ( carretPos.x < scrollPos )
         scrollPos = carretPos.x;
   }
}

void GuiEditBox::moveRight(bool ctrl)
{
   if ( ctrl )
   {
      const char* ptext = getCaption().c_str();
      skipToWhitespace(ptext, true);
      skipWhitespace(ptext, true);
   }
   else
   {
      int len = getLineLength(carretPos.y);
      if ( getMultiLine() )
      {
         if ( carretPos.x == len )
         {
            if ( carretPos.y < getLineCount() - 1 )
            {
               carretPos.x = 0;
               ++carretPos.y;
            }
         }
         else
            ++carretPos.x;
      }
      else
         if ( carretPos.x < len - 1 )
            ++carretPos.x;
   }

   if ( carretPos.x < scrollPos )
      scrollPos = carretPos.x;
   else if ( carretPos.x > scrollPos + maxChars )
      scrollPos += (carretPos.x - scrollPos - maxChars);
}

void GuiEditBox::moveUp()
{
   if ( carretPos.y > 0 )
      doMove(-1);
}

void GuiEditBox::moveDown()
{
   if ( carretPos.y < getLineCount() - 1 )
      doMove(1);
}

void GuiEditBox::doMove(int direction)
{
   carretPos.y += direction;

   int lineLen = getLineLength(carretPos.y);
   if ( carretPos.x > lineLen )
   {
      carretPos.x = lineLen;

      if ( carretPos.x < scrollPos )
         scrollPos = carretPos.x;
   }
}

void GuiEditBox::skipWhitespace(const char* ptext, bool forward)
{
   int pos = carretToStringPos();
   if ( forward )
   {
      while ( isspace(ptext[pos]) )
      {
         if ( ptext[pos] == '\n' )
         {
            carretPos.x = 0;
            ++carretPos.y;
         }
         else
            carretPos.x++;

         pos++;
      }
   }
   else
   {
      while ( carretPos.x > 0 && isspace(ptext[pos-1]) )
      {
         --carretPos.x;
         --pos;
      }
   }
}

void GuiEditBox::skipToWhitespace(const char* ptext, bool forward)
{
   int pos = carretToStringPos();
   if ( forward )
   {
      int len = strlen(ptext);
      while ( pos < len && !isspace(ptext[pos]) )
      {
         ++carretPos.x;
         ++pos;
      }
   }
   else
   {
      while ( carretPos.x > 0 && !isspace(ptext[pos - 1]) )
      {
         --carretPos.x;
         --pos;
      }
   }
}
