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
#ifndef _GUILISTBOX_H_
#define _GUILISTBOX_H_

#include <vector>
#include <string>

#include "gui/headercontrol/guiheadercontrol.h"

#include "guicontrol.h"
#include "guilistboxitem.h"
#include "guilistboxsubitem.h"

#include "listbox/guilistboxmouselistener.h"

class GuiEditBox;
class GuiComboBox;
class GuiScrollBar;

class GuiListBox : public GuiControl
{
public:
   typedef std::vector<GuiListBoxItem*> Items;
   typedef enum { EEditBox, EComboBox } EditType;

   DESIGNER_REGISTRATION(GuiListBox)

                  GuiListBox();
   virtual        ~GuiListBox();

   bool           isEditing() const;
   void           setEditing(bool editing);

   virtual void   sendMessage(GuiId id, GuiEvent event, int param1);

   virtual void   onMouseWheel(const GuiPoint& point, int direction, int flags);
   virtual void   onKeyDown (int which, bool shift, bool ctrl, bool alt);
   virtual void   onKeyUp(int which);

   bool           hasCurSel() const;
   void           setCurSel(int sel);
   int            getCurSel() const;

   void           setItemText(int index, const char* text, int subitem = 0);
   void           setItemData(int index, Uint32 data);

   const char*    getItemText(int index) const;
   Uint32         getItemData(int index) const;
   Uint32         getItemCount() const;

   int            addString(const char* str);
   void           removeAt(int index);
   void           removeAll();

   GuiHeaderCtrl&       header();
   const GuiHeaderCtrl& header() const;
   bool                 hasHeader() const;

   void           setEditableColumn(int column);
   void           cancelEdit();

   void           scrollTo(int idx);

   void selectItem(const Point& location);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);
   virtual void   drawItem(int index, int posX, int posY, const GuiGraphics& graphics);
   virtual void   onResize(int width, int height);

   virtual void   initializeEventHandlerDefinitions();

   virtual int       measureItem(int index);
   virtual void      applyChange(const std::string& text);
   virtual EditType  editType(int index) const;
   virtual void      fillEdit(int row);
   virtual void      fillCombo(int row);

   void           drawSelection(int index, int x, int y);

   int            calculateItemUnderCursor(const GuiPoint& point);

   void           onScroll(int pos);

   void           onEditLostFocus();
   void           onComboLostFocus();

private:
   friend class GuiListBoxMouseListener;

   void           initializeHeader();
   void           initializeScrollbar();

   void           calculateCapacity();
   bool           isItemVisible(int index);
   void           ensureItemVisible(int index);
   int            determineVisibleItemCount();
   GuiRect        getSubItemBounds(int row, int column);
   void           selectColumn(int row, int x);
   GuiRect        calculateControlRect(int row, int column);

   static void    initializeControls();

protected:

   Items          lines;
   int            maxLines;
   int            maxChars;
   GuiScrollBar*  vertBar;
   GuiHeaderCtrl* _header;
   Uint32         scrollPos;
   int            curSel;
   int            curSelColumn;
   Uint32         visibleItemCount;
   bool           dirty;
   bool           _editing;

   static GuiComboBox*     _combo;
   static GuiEditBox*      _edit;

private:
   DECLARE_PROPERTIES
   BOOL_PROPERTY(Linebreak)


   GuiListBoxMouseListener mMouseListener;
};

#ifdef JENGINE_INLINE
#  include "guilistbox.inl"
#endif

#endif
