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
#include "guipropertylist.h"

#include "core/defines.h"

#include "gui/combobox/guicombobox.h"

GuiPropertyList::GuiPropertyList():
   GuiListBox(),
   MPProperties(NULL),
   _pwindow(NULL)
{
}

GuiPropertyList::~GuiPropertyList()
{
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

const GuiProperties& GuiPropertyList::Properties() const
{
   return *MPProperties;
}

GuiProperties& GuiPropertyList::Properties()
{
   return *MPProperties;
}

void GuiPropertyList::Properties(GuiProperties& properties)
{
   cancelEdit();
   removeAll();

   MPProperties = &properties;
   for ( int i = 0; i < Properties().Size(); ++i )
   {
      GuiPropertyBase& property = Properties()[i];
      addProperty(property, i);
   }
}

void GuiPropertyList::window(GuiWnd* pwnd)
{
   _pwindow = pwnd;
}

void GuiPropertyList::addProperty(GuiPropertyBase& property, int propertyindex)
{
   int index = addString(property.Name().c_str());
   setItemData(index, propertyindex);

   if ( property.Type() == GuiPropertyBase::EString )
   {
      GuiProperty<std::string>& strproperty = dynamic_cast<GuiProperty<std::string>&>(property);
      setItemText(index, strproperty.Value().c_str(), 1);
   }
   else if ( property.Type() == GuiPropertyBase::EBool )
   {
      GuiProperty<bool>& boolproperty = dynamic_cast<GuiProperty<bool>&>(property);
      setItemText(index, boolproperty.Value() ? "true" : "false", 1);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Overloads interface
//////////////////////////////////////////////////////////////////////////

void GuiPropertyList::applyChange(const std::string& text)
{
   int propertyindex = getItemData(getCurSel());
   GuiPropertyBase& property = Properties()[propertyindex];

   property.fromString(text.c_str());
}

GuiListBox::EditType GuiPropertyList::editType(int index) const
{
   int propertyindex = getItemData(index);
   const GuiPropertyBase& property = Properties()[propertyindex];

   if ( property.Type() == GuiPropertyBase::EBool )
   {
      return EComboBox;
   }
   else if ( property.Type() == GuiPropertyBase::EComplex )
   {
      ASSERT("TODO!");
   }

   return EEditBox;
}

void GuiPropertyList::fillCombo(int row)
{
   _combo->removeAll();
   _combo->addString("true");
   _combo->addString("false");

   int propertyindex = getItemData(getCurSel());
   GuiPropertyBase& propertybase = Properties()[propertyindex];

   GuiProperty<bool>& property = dynamic_cast<GuiProperty<bool>&>(propertybase);
   if ( property.Value() )
      _combo->setCurSel(0);
   else
      _combo->setCurSel(1);
}
