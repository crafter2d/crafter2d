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
#include "guitreebox.h"
#ifndef JENGINE_INLINE
#  include "guitreebox.inl"
#endif

#include "../containers/treedepthfirstiterator.h"

#include "../scriptmanager.h"

#include "guidesigner.h"
#include "guidefines.h"
#include "guifont.h"
#include "guieventhandler.h"
#include "guieventhandlers.h"
#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"
#include "guimanager.h"
#include "guiscopedtransform.h"
#include "guitext.h"

REGISTER_DESIGNER(GuiTreeBox, GuiTreeBoxId, "Treebox", 40, 15, 392)

GuiTreeBox::GuiTreeBox():
   GuiControl(),
   mMouseListener(*this),
   _items(),
   _selections()
{
}

GuiTreeBox::~GuiTreeBox()
{
}

//////////////////////////////////////////////////////////////////////////
// - get/set interface
//////////////////////////////////////////////////////////////////////////

bool GuiTreeBox::hasSelection() const
{
   return !_selections.empty();
}

const GuiTreeBox::Handle GuiTreeBox::getSelection() const
{
   ASSERT(_selections.size() > 0)
   return *_selections.front();
}

GuiTreeBox::Handle GuiTreeBox::getSelection()
{
   ASSERT(_selections.size() > 0)
   return *_selections.front();
}

//////////////////////////////////////////////////////////////////////////
// - creation
//////////////////////////////////////////////////////////////////////////

void GuiTreeBox::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   GuiTreeBoxItem* pitem = new GuiTreeBoxItem();
   pitem->setText("'Designer'");
   pitem->setIcon("folder.png");

   GuiTreeBoxItem* pchild = new GuiTreeBoxItem();
   pchild->setText("Dialogs");
   pchild->setIcon("application_double.png");

   GuiTreeBoxItem* pdialog = new GuiTreeBoxItem();
   pdialog->setText("des_openfile");
   pdialog->setIcon("application_form.png");

   _items.insert(NULL, *pitem);

   HandlePtr pnode = _items.findElement(*pitem);
   _items.insert(pnode, *pchild);

   pnode = _items.findElement(*pchild);
   _items.insert(pnode, *pdialog);
   
   addMouseListener(mMouseListener);
}

//////////////////////////////////////////////////////////////////////////
// - painting
//////////////////////////////////////////////////////////////////////////

void GuiTreeBox::paint(Uint32 tick, const GuiGraphics& graphics)
{
   if ( _items.hasRoot() )
   {
      int y = 5;
      Handle root = _items.getRoot();

      GuiScopedTransform transform(*this);
      recursePaint(root, 5, y, graphics);
   }
}

void GuiTreeBox::recursePaint(Handle item, int x, int& y, const GuiGraphics& graphics)
{
   drawItem(item, x, y, graphics);

   y += 16;

   if ( item.isExpanded() )
   {
      TreeNode<GuiTreeBoxItem>::Children& children = item.getChildren();
      ListIterator< TreeNode<GuiTreeBoxItem> > it(children);
      while ( it.isValid() )
      {
         recursePaint(it.item(), x + 20, y, graphics);
         ++it;
      }
   }
}

void GuiTreeBox::drawItem(const Handle item, int x, int y, const GuiGraphics& graphics)
{
   graphics.setColor(1, 1, 1);
   paintCheck(item, x, y + getParent()->getFont()->getBaseHeight(), graphics);

   if ( item.getData().hasIcon() )
      graphics.drawImage(*item.getData().getIcon(), GuiRect(x+15, x+31, y, y+16));

   paintText(item, x, y, graphics);
}

void GuiTreeBox::paintText(const Handle item, int x, int y, const GuiGraphics& graphics)
{
   const std::string& title = item.getData().getText();
   if ( item.getData().isSelected() )
   {
      int width = getFont()->getAverageWidth() * title.length() + 34;
      GuiRect rect(x+34, x+width, y + 1, y + getFont()->getHeight() - 1);

      graphics.setColor(0, 0, 1);
      graphics.drawRect(rect);
      graphics.setColor(1, 1, 1);
      GuiText::printfn(*getFont(), x + 34, y+getFont()->getBaseHeight(), title.c_str(), title.size());
   }
   else
   {
      graphics.setColor(GuiManager::getInstance().getDefaultTextColor());
      GuiText::printfn(*getFont(), x + 34, y+getFont()->getBaseHeight(), title.c_str(), title.size());
   }
}

void GuiTreeBox::paintCheck(const Handle item, int x, int y, const GuiGraphics& graphics)
{
   if ( item.hasChildren() )
   {
      graphics.drawWireRect(GuiRect(x, x+8, y, y-8));

      glBegin(GL_LINES);
         glVertex2i(x+2, y-4);
         glVertex2i(x+7, y-4);
         if ( !item.isExpanded() )
         {
            glVertex2i(x+4, y-2);
            glVertex2i(x+4, y-7);
         }
      glEnd();
   }
}

//////////////////////////////////////////////////////////////////////////
// - event handler interface
//////////////////////////////////////////////////////////////////////////

void GuiTreeBox::initializeEventHandlerDefinitions()
{
   GuiControl::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiTreeSelChangeEvent, "onSelectionChanged");
   pdefinition->addArgument("selection");
   getEventHandlerDefinitions().add(pdefinition);
}

//////////////////////////////////////////////////////////////////////////
// - operations
//////////////////////////////////////////////////////////////////////////

void GuiTreeBox::insert(HandlePtr position, GuiTreeBoxItem& item)
{
   _items.insert(position, item);
}

void GuiTreeBox::select(const GuiPoint& point)
{
   int index = (point.y - 5) / 16;
   HandlePtr pitem = findItemByIndex(index);
   if ( pitem != NULL )
   {
      if ( isAboveCheckbox(*pitem, point.x) )
      {
         if ( pitem->isExpanded() )
            pitem->collapse();
         else
            pitem->expand();
      }
      else
      {
         select(pitem);
      }
   }
}

void GuiTreeBox::select(HandlePtr pitem)
{
   deselectAll();
   _selections.push_back(pitem);

   GuiTreeBoxItem& item = pitem->getData();
   item.setSelected(true);

   GuiEventHandler* phandler = getEventHandlers().findByEventType(GuiTreeSelChangeEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, "GuiTreeBox");
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam(findIndex(pitem));
      script.run(1);
   }
}

void GuiTreeBox::deselectAll()
{
   while ( !_selections.empty() )
   {
      HandlePtr pitem = _selections.front();
      pitem->getData().setSelected(false);

      _selections.pop_front();
   }
}

bool GuiTreeBox::isAboveCheckbox(Handle item, int x)
{
   int depth = item.getDepth();
   int xpos = depth * 20 + 5;

   return x >= xpos && x <= xpos + 8;
}

GuiTreeBox::HandlePtr GuiTreeBox::findItemByIndex(int index)
{
   int pos = 0;
   TreeDepthFirstIterator<GuiTreeBoxItem> iterator(_items);
   while ( pos < index && iterator.isValid() )
   {
      GuiTreeBoxItem& item = iterator.item();
      ++iterator;

      ++pos;
   }

   return iterator.isValid() ? reinterpret_cast< TreeNode<GuiTreeBoxItem>* >(iterator.key()) : NULL;
}

//////////////////////////////////////////////////////////////////////////
// - search interface
//////////////////////////////////////////////////////////////////////////

GuiTreeBoxItem* GuiTreeBox::findItemByText(const std::string& text)
{
   TreeDepthFirstIterator<GuiTreeBoxItem> it(_items);
   while ( it.isValid() )
   {
      GuiTreeBoxItem& item = it.item();
      if ( item.getText() == text )
      {
         return &item;
      }

      ++it;
   }

   return NULL;
}

int GuiTreeBox::findIndex(HandlePtr pitem)
{
   TreeDepthFirstIterator<GuiTreeBoxItem> it(_items);
   int index = 0;

   while ( it.isValid() )
   {
      if ( it.key() == pitem)
         return index;

      ++index;
      ++it;
   }

   return -1;
}
