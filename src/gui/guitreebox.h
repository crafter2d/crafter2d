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
#ifndef GUI_TREEBOX_H_
#define GUI_TREEBOX_H_

#include "core/containers/tree.h"

#include "treebox/guitreeboxmouselistener.h"

#include "guicontrol.h"
#include "guitreeboxitem.h"

class GuiTreeBox : public GuiControl
{
public:
   typedef Tree<GuiTreeBoxItem> Items;
   typedef TreeNode<GuiTreeBoxItem>& Handle;
   typedef TreeNode<GuiTreeBoxItem>* HandlePtr;
   typedef std::list< HandlePtr >  SelectionList;

   DESIGNER_REGISTRATION(GuiTreeBox)

   GuiTreeBox();
   virtual ~GuiTreeBox();

 // operations
   void insert(HandlePtr position, GuiTreeBoxItem& item);

 // selection interface
         bool     hasSelection() const;
   const Handle   getSelection() const;
         Handle   getSelection();
   
         void     deselectAll();

 // search interface
   GuiTreeBoxItem*   findItemByText(const std::string& text);

protected:
 // get/set interface
   Items&   getTree();

 // operations
   virtual void      onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);

   virtual void      initializeEventHandlerDefinitions();

   virtual void      paint(Uint32 tick, const GuiGraphics& graphics);
           void      recursePaint(Handle root, int x, int& y, const GuiGraphics& graphics);

   virtual void      drawItem(const Handle item, int x, int y, const GuiGraphics& graphics);
           void      paintText(const Handle item, int x, int y, const GuiGraphics& graphics);
           void      paintCheck(const Handle item, int x, int y, const GuiGraphics& graphics);

           void      select(const GuiPoint& point);
           void      select(HandlePtr pitem);

           bool      isAboveCheckbox(Handle item, int x);
           HandlePtr findItemByIndex(int index);
           int       findIndex(HandlePtr pitem);

private:
   friend class GuiTreeBoxMouseListener;

   GuiTreeBoxMouseListener mMouseListener;
   Items          _items;
   SelectionList  _selections;
};

#ifdef JENGINE_INLINE
#  include "guitreebox.inl"
#endif

#endif
