/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _GUICOMBOBOX_H_
#define _GUICOMBOBOX_H_

#include <vector>
#include <string>
#include "guicontrol.h"
#include "guibutton.h"
#include "guilistbox.h"

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

   virtual void      onKeyDown (int which, bool shift, bool ctrl, bool alt);
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

   GuiListBox*          options;
   GuiButton*           drop;
   GuiPopupDecorator*   _list;
   bool                 _listvisible;
   std::string          callbackFnc;
   GuiRect              selectionbox;
};

/// @author Jeroen Broekhuizen
///
/// Represents a color combo box which can be used to select certain colors
/// which can be added using the addColor function. No strings should be added
/// to the combobox.
class GuiColorComboBox: public GuiComboBox
{
protected:
   class GuiColorListBox: public GuiListBox
   {
   public:
      typedef std::vector<GuiColor> Colors;

      GuiColorListBox() : colors() {}

      void              addColor(GuiColor color);
      const GuiColor&   getColor(int idx) const;

   protected:
      virtual void      drawItem(int idx, int posX, int posY, const GuiGraphics& graphics);

      Colors colors;
   };

public:
                  GuiColorComboBox();

   void           addColor(GuiColor color);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);
};

#ifdef JENGINE_INLINE
#  include "guicombobox.inl"
#endif

#endif
