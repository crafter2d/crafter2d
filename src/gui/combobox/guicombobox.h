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
#ifndef _GUICOMBOBOX_H_
#define _GUICOMBOBOX_H_

#include <vector>
#include <string>

#include "gui/guicontrol.h"
#include "gui/guilistbox.h"

#include "guicomboboxkeylistener.h"

class GuiButton;
class GuiPopupDecorator;

/// @author Jeroen Broekhuizen
///
/// Represents a combo box to use in this canvas. It uses a listbox to represent the
/// dropdown box.
class GuiComboBox: public GuiControl
{
public:
   DESIGNER_REGISTRATION(GuiComboBox)

                     GuiComboBox();
                     GuiComboBox(int id, const GuiRect& rect, const char* caption = "", int style=0, GuiWnd* parent=NULL);

   void              setCallback(const char* fnc);
   void              setItemText(int index, const char* text);
   void              setItemData(int index, Uint32 data);
   void              setCurSel(int index);

   const char*       getItemText(int index) const;
   Uint32            getItemData(int index) const;
   int               getCurSel() const;

   virtual void      sendMessage(GuiId, GuiEvent event, int param1=0);
   virtual void      onKillFocus(GuiWnd* newCtrl);

   void              removeAll();
   int               addString(const char* str);

protected:
   virtual void      onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void      onResize(int width, int height);

   virtual void      paint(Uint32 tick, const GuiGraphics& graphics);

   void              displayList(bool display);

   void              onDown();
   void              onSelChange(int newSel);
   void              onListLostFocus();

   GuiComboBoxKeyListener mKeyListener;
   GuiListBox*          options;
   GuiButton*           drop;
   GuiPopupDecorator*   _list;
   bool                 _listvisible;
   std::string          callbackFnc;
   GuiRect              selectionbox;
};

#ifdef JENGINE_INLINE
#  include "guicombobox.inl"
#endif

#endif
