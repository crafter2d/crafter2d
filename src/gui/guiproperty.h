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
#ifndef GUIPROPERTY_H
#define GUIPROPERTY_H

#include "guipropertybase.h"

template <class T>
class GuiProperty : public GuiPropertyBase
{
public:
   GuiProperty(std::string name, const T& defaultvalue, GuiPropertyBase::PropertyType type);
   virtual ~GuiProperty();
   
   void Initialize(std::string name, const T& defaultvalue, GuiPropertyBase::PropertyType type);
   
   void operator()(const T& value);
   const T& operator()() const;
         T& operator()();

// Get/set interface
   const T& Value() const;
         T& Value();
   void     Value(const T& value);

private:
   T MValue;
   T MDefault;
};

template <class T>
GuiProperty<T>::GuiProperty(std::string name, const T& defaultvalue, GuiPropertyBase::PropertyType type):
   GuiPropertyBase(name, type),
   MValue(defaultvalue),
   MDefault(defaultvalue)
{
}

template <class T>
GuiProperty<T>::~GuiProperty()
{
}

template <class T>
void GuiProperty<T>::operator()(const T& value)
{
   Value(value);
}

template <class T>
const T& GuiProperty<T>::operator()() const
{
   return Value();
}

template <class T>
T& GuiProperty<T>::operator()()
{
   return Value();
}

template <class T>
const T& GuiProperty<T>::Value() const
{
   return MValue;
}

template <class T>
T& GuiProperty<T>::Value()
{
   return MValue;
}

template <class T>
void GuiProperty<T>::Value(const T& value)
{
   MValue = value;
   callback();
}

#endif
