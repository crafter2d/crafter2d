/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef GUI_COMBOBOX_KEY_LISTENER_H
#define GUI_COMBOBOX_KEY_LISTENER_H

#include "gui/input/keylistener.h"

class GuiComboBox;

class GuiComboBoxKeyListener : public KeyListener
{
public:
   explicit GuiComboBoxKeyListener(GuiComboBox& combobox);
   virtual ~GuiComboBoxKeyListener();

   virtual void onKeyPressed(const KeyEvent& event);

private:
   GuiComboBox& mComboBox;
};

#endif
