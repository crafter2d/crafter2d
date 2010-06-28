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
#include "guilistbox.h"
#ifndef JENGINE_INLINE
#  include "guilistbox.inl"
#endif

#include "../script.h"
#include "../scriptmanager.h"

#include "gui/combobox/guicombobox.h"
#include "gui/edit/guiedit.h"
#include "gui/headercontrol/guiheadercontrol.h"
#include "gui/headercontrol/guiheadercolumn.h"

#include "guimanager.h"
#include "guiscrollbar.h"
#include "guilistboxitem.h"
#include "guilistboxsubitem.h"
#include "guitextblock.h"
#include "guifont.h"
#include "guidesigner.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"

GuiComboBox* GuiListBox::_combo = NULL;
GuiEditBox*  GuiListBox::_edit  = NULL;

REGISTER_DESIGNER(GuiListBox, GuiListBoxId, "Listbox", 40, 30, 392)

EVENT_MAP_BEGIN(GuiListBox)
   ON_SB_UP(1, onScroll)
   ON_SB_DOWN(1, onScroll)
   ON_LOST_FOCUS(11, onComboLostFocus)
   ON_LOST_FOCUS(12, onEditLostFocus)
EVENT_MAP_END()

IMPLEMENT_PROPERTIES(GuiListBox, GuiControl)
   CONSTRUCT_BOOL_PROPERTY(Linebreak, "Linebreak", false);
}

void GuiListBox::initializeEventHandlerDefinitions()
{
   GuiControl::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiListSelChangeEvent, "onSelectionChanged");
   pdefinition->addArgument("selection");
   getEventHandlerDefinitions().add(pdefinition);

   pdefinition = new GuiEventHandlerDefinition(GuiListKeyPressEvent, "onKeyPress");
   pdefinition->addArgument("key");
   getEventHandlerDefinitions().add(pdefinition);
}

GuiListBox::GuiListBox():
   GuiControl(),
   INIT_PROPERTY(Linebreak),
   maxLines(0),
   maxChars(0),
   vertBar(NULL),
   _header(NULL),
   scrollPos(0),
   curSel(-1),
   curSelColumn(-1),
   visibleItemCount(0),
   dirty(true),
   _editing(false)
{
}

GuiListBox::~GuiListBox()
{
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

void GuiListBox::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   calculateCapacity();

   initializeControls();
   initializeHeader();
   initializeScrollbar();
}

void GuiListBox::initializeControls()
{
   if ( _combo == 0 && _edit == 0 )
   {
      _combo = new GuiComboBox();
      _combo->create(11, GuiRect(0,1,0,1), "", GUI_BACKGROUND|GUI_BORDER|GUI_NOTIFYPARENT);
      _combo->setVisible(false);

      _edit = new GuiEditBox();
      _edit->create(12, GuiRect(0,1,0,1), "", GUI_BACKGROUND|GUI_BORDER|GUI_NOTIFYPARENT);
      _edit->setVisible(false);
   }
}

void GuiListBox::initializeHeader()
{
   _header = new GuiHeaderCtrl();
   _header->create(2, 264, this);                  // transfer ownership

   if ( !hasHeader() )
   {
      _header->addColumn(m_frameRect.getWidth());
      _header->setVisible(false);
   }
}

void GuiListBox::initializeScrollbar()
{
   const GuiRect& rect = getWindowRect();
   int w = rect.getWidth();

   if ( vertBar != NULL )
   {
      removeChild(vertBar);
      delete vertBar;
   }

   vertBar = new GuiScrollBar();
   vertBar->create(1, GuiRect(w-15, w, 1, rect.getHeight()-2), "", GUI_VISIBLE|GUI_BACKGROUND, this);
   vertBar->setScrollInfo(maxLines);
}

//////////////////////////////////////////////////////////////////////////
// - Painting
//////////////////////////////////////////////////////////////////////////

void GuiListBox::paint (Uint32 tick, const GuiGraphics& graphics)
{
   if ( lines.size() > 0 )
   {
      // find out how much messages are visible
      Uint32 end = determineVisibleItemCount();

      // now render the text
      int posY = m_frameRect.top() + (hasHeader() ? GuiHeaderCtrl::height() : 0);
      int posX = m_frameRect.left();

      for (int i = scrollPos; i < scrollPos + end; i++)
      {
         int height = measureItem(i);
         if ( height > 0 )
         {
            drawItem(i, posX, posY, graphics);
            posY += height;
         }
      }
   }
}

void GuiListBox::drawItem(int index, int posX, int posY, const GuiGraphics& graphics)
{
   graphics.setColor(GuiManager::getInstance().getDefaultTextColor());

   GuiListBoxItem& item = *lines[index];
   item.paint(GuiPoint(posX, posY), graphics);

   drawSelection(index, posX, posY);
}

void GuiListBox::drawSelection(int index, int x, int y)
{
   if (curSel == index && !hasHeader())
   {
      glColor3f(1,1,1);

      // calculate the corner coordinates
      int pLX = x + 3;
      int pRX = pLX + (m_frameRect.getWidth() - 6 - 16);
      int pUY = y + 3;
      int pBY = y + (*lines[index])[0].block().height() + 2;

      if ( hasHeader() )
      {
         pUY += GuiHeaderCtrl::height();
         pBY += GuiHeaderCtrl::height();
      }

      // render the selection rectangle
      static unsigned short pattern = 43690;
      glLineStipple(3, pattern);
      glEnable(GL_LINE_STIPPLE);
      glBegin(GL_LINE_STRIP);
      glVertex2i(pLX, pUY); glVertex2i(pRX, pUY);
      glVertex2i(pRX, pBY); glVertex2i(pLX, pBY);
      glVertex2i(pLX, pUY);
      glEnd();
      glDisable(GL_LINE_STIPPLE);
   }
}

int GuiListBox::measureItem(int idx)
{
   return (*lines[idx])[0].block().height();
}

//////////////////////////////////////////////////////////////////////////
// - Interface methods
//////////////////////////////////////////////////////////////////////////

int GuiListBox::addString(const char* str)
{
   if ( font == NULL )
      return -1;

   dirty = true;

   lines.push_back(new GuiListBoxItem(this, str));
   vertBar->setScrollRange(lines.size());

   return (lines.size() - 1);
}

void GuiListBox::setCurSel(int sel)
{
   assert(parent != 0 && "Listbox has no parent!");

   if ( sel != curSel )
   {
      if ( sel >= 0 && sel < lines.size() )
      {
         if ( curSel >= 0 )
            lines[curSel]->selected(false);

         lines[sel]->selected(true);
         curSel = sel;

         callback(LB_SELCHANGE);
      }
      else
      {
         if ( curSel >= 0 )
            lines[curSel]->selected(false);
         curSel = -1;
      }
   }
}

void GuiListBox::setItemText(int index, const char* text, int subitem)
{
   assert(index >= 0 && index < lines.size() && "setItemText: Invalid item index.");
   assert(subitem >= 0 && subitem < header().count() && "setItemText: invalid subitem index.");

   GuiListBoxItem& item = *(lines[index]);
   if ( item.count() <= subitem )
   {
      item.resize(subitem+1); // zero-based
   }

   item[subitem].text(text);
}

void GuiListBox::setEditableColumn(int column)
{
   assert( column >= 0 && column < header().count() && "GuiListBox.setEditableColumn: invalid column number." );
   header()[column].editable(true);
}

void GuiListBox::scrollTo(int idx)
{
   ensureItemVisible(idx);
}

void GuiListBox::onResize(int width, int height)
{
   initializeScrollbar();

   if ( !hasHeader() )
   {
      GuiHeaderCtrl* pheader = dynamic_cast<GuiHeaderCtrl*>(getItemById(2));
      (*pheader)[0].resize(width);
   }
}

void GuiListBox::removeAll()
{
   setCurSel(-1);

   for ( int index = 0; index < lines.size(); ++index )
   {
      GuiListBoxItem* pitem = lines[index];
      delete pitem;
   }

   lines.clear();
}

//////////////////////////////////////////////////////////////////////////
// - Input messages
//////////////////////////////////////////////////////////////////////////

int GuiListBox::onLButtonUp (const GuiPoint& point, int flags)
{
   if ( GuiControl::onLButtonUp(point, flags) == JENGINE_MSG_HANDLED )
   {
      return JENGINE_MSG_HANDLED;
   }
   else
   {
      if ( !lines.empty() )
      {
         int item = calculateItemUnderCursor(point);

         if ( isEditing() && item != getCurSel() )
         {
            if ( editType(getCurSel()) == EEditBox )
               onEditLostFocus();
            else
               onComboLostFocus();
         }

         if ( item != -1 )
         {
            setCurSel(item);
            selectColumn(item, point.x);

            GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiListSelChangeEvent);
            if ( phandler != NULL )
            {
               ScriptManager& mgr = ScriptManager::getInstance();
               Script& script = mgr.getTemporaryScript();
               script.setSelf(this, "GuiListBox");
               script.prepareCall(phandler->getFunctionName().c_str());
               script.addParam(item);
               script.run(1);
            }
            else
               parent->sendMessage(m_id, GuiListSelChangeEvent, item);
         }
         else
         {
            setCurSel(-1);
         }
      }

      return JENGINE_MSG_HANDLED;
   }

   return JENGINE_MSG_UNHANDLED;
}

void GuiListBox::onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flags)
{
   GuiPoint p(point);
   windowToClient(p);

   if ( header().hitTest(p) )
   {
      header().onMouseMove(point, rel, flags);
   }
}

void GuiListBox::onMouseWheel(const GuiPoint& point, int direction, int flags)
{
   int pos = vertBar->getScrollPosition();
   if ( direction < 0 )
   {
      if ( scrollPos > 0 )
         scrollPos--;
   }
   else
   {
      if ( ++scrollPos > vertBar->getScrollRange() )
         scrollPos = vertBar->getScrollRange();
   }
   vertBar->setScrollPosition(scrollPos);
}

void GuiListBox::onKeyDown (int which, bool shift, bool ctrl, bool alt)
{
   if ( which == SDLK_RETURN && _edit->getVisible() )
   {
      // stop editing
      setFocus();
   }
}

void GuiListBox::onKeyUp(int which)
{
   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiListKeyPressEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiListBox");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam(which);
      script.run(1);
   }
}

void GuiListBox::onEditLostFocus()
{
   if ( isEditing() )
   {
      std::string text = _edit->getCaption();

      (*lines[curSel])[curSelColumn].text(text);
      applyChange(text);

      curSelColumn = -1;
      setEditing(false);

      _edit->setParent(0);
      _edit->setVisible(false);
   }
}

void GuiListBox::onComboLostFocus()
{
   if ( isEditing() )
   {
      std::string text = _combo->getItemText(_combo->getCurSel());

      (*lines[curSel])[curSelColumn].text(text);
      applyChange(text);

      curSelColumn = -1;
      setEditing(false);

      _combo->setParent(NULL);
      _combo->setVisible(false);
   }
}

void GuiListBox::applyChange(const std::string& text)
{
}

//////////////////////////////////////////////////////////////////////////
// - Helper functions
//////////////////////////////////////////////////////////////////////////

void GuiListBox::selectColumn(int row, int x)
{
   int pos = 0;
   for (int i=0; i < header().count(); ++i)
   {
      const GuiHeaderColumn& column = header()[i];
      if ( column.editable() )
      {
         if ( x >= pos && x < pos+column.width() )
         {
            GuiRect rect = calculateControlRect(row, i);
            if ( i != curSelColumn )
            {
               curSelColumn = i;

               GuiControl* pctrl = NULL;
               if ( editType(row) == EEditBox )
                  pctrl = _edit;
               else
                  pctrl = _combo;

               pctrl->setParent(this);
               pctrl->setWindowPos(rect.left(), rect.top());
               pctrl->resizeWindow(rect.getWidth(), rect.getHeight());
               pctrl->setVisible(true);
               pctrl->setFocus();

               if ( pctrl == _edit )
                  fillEdit(row);
               else
                  fillCombo(row);

               setEditing(true);
            }
            return;
         }
      }

      pos += column.width();
   }
}

void GuiListBox::cancelEdit()
{
   if ( isEditing() )
   {
      GuiControl* pcontrol = NULL;
      if ( _edit->getVisible() )
         pcontrol = _edit;
      else
         pcontrol = _combo;

      curSelColumn = -1;
      setEditing(false);

      pcontrol->setParent(NULL);
      pcontrol->setVisible(false);
      setFocus();
   }
}

void GuiListBox::fillEdit(int row)
{
   _edit->setCaption((*lines[row])[curSelColumn].block().text().c_str());
}

void GuiListBox::fillCombo(int row)
{
}

void GuiListBox::calculateCapacity()
{
   maxLines = (m_frameRect.getHeight() - (hasHeader() ? GuiHeaderCtrl::height() : 0)) / font->getHeight();
   maxChars = (m_frameRect.getWidth() - 16) / font->getAverageWidth();
}

bool GuiListBox::isItemVisible(int index)
{
   if ( index < scrollPos || index >= scrollPos + determineVisibleItemCount() )
   {
      return false;
   }

   return true;
}

void GuiListBox::ensureItemVisible(int index)
{
   assert( index >= 0 && index < lines.size() && "Invalid index." );
   if ( !isItemVisible(index) )
   {
      if ( scrollPos >= index )
      {
         scrollPos = index;
      }
      else
      {
         scrollPos = index;
         int remainingHeight = (m_frameRect.getHeight() - (hasHeader() ? GuiHeaderCtrl::height() : 0)) -
                               (*lines[index])[0].block().height();

         for ( int i = index-1; i >= 0; --i )
         {
            remainingHeight -= (*lines[i])[0].block().height();
            if ( remainingHeight > 0 )
               scrollPos--;
            else
               break;
         }
      }

      vertBar->setScrollPosition(scrollPos);
   }
}

int GuiListBox::determineVisibleItemCount()
{
   if ( true )
   {
      int fullheight = (m_frameRect.getHeight() - (hasHeader() ? GuiHeaderCtrl::height() : 0));
      int currentheight = 0;

      visibleItemCount = 0;
      for ( int i = scrollPos; i < lines.size(); ++i )
      {
         currentheight += (*lines[i])[0].block().height();
         if ( currentheight >= fullheight )
         {
            break;
         }
         else
         {
            visibleItemCount++;
         }
      }

      dirty = false;
   }

   return visibleItemCount;
}

GuiRect GuiListBox::calculateControlRect(int row, int column)
{
   GuiRect rect = getSubItemBounds(row, column);
   if ( rect.right() >= m_frameRect.getWidth()-16 )
   {
      rect.right(m_frameRect.getWidth()-19);
   }

   return rect;
}

GuiRect GuiListBox::getSubItemBounds(int row, int column)
{
   int x = 0;
   for (int i=0; i < column; ++i)
   {
      x += header()[i].width();
   }

   int y = 0;
   for (int i=0; i < row; ++i)
   {
      y += (*lines[i])[0].block().height();
   }

   if ( hasHeader() )
   {
      y += 16;
   }

   return GuiRect(x, x+header()[column].width(), y+2, y+2+(*lines[row])[0].block().height());
}

int GuiListBox::calculateItemUnderCursor(const GuiPoint& point)
{
   int item = 0;
   int height = hasHeader() ? header().height() : 0;
   for ( int i = scrollPos; i < scrollPos + determineVisibleItemCount(); ++i )
   {
       //height += (*lines[i])[0].block().height();
      height += measureItem(i);
      if ( point.y < height )
      {
         return i;
      }
   }
   return -1;
}
