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
#ifndef GUIPROPERTYBASE_H
#define GUIPROPERTYBASE_H

#include <string>

class GuiProperties;
class GuiWnd;
class TiXmlElement;

/**
@author Jeroen Broekhuizen
*/
class GuiPropertyBase
{
public:
   typedef void (*CallbackFnc)(GuiWnd* pwnd);
   typedef enum { EString, EInt, EFloat, EBool, EComplex, EInvalid } PropertyType;

   GuiPropertyBase(std::string name, PropertyType type);
   virtual ~GuiPropertyBase();
   
// Get/set interface
   const std::string& Name() const;
         std::string& Name();

   PropertyType       Type() const;

   GuiProperties&     container();
   void               container(GuiProperties* pcontainer);

// Registration
   void               registerCallback(CallbackFnc callback);
   void               callback();

// Conversion
   std::string    toString() const;
   void           fromString(const char* pvalue);

   TiXmlElement*  toXML() const;
   void           fromXML(const TiXmlElement* pelement);

private:
   std::string    MName;
   PropertyType   MType;
   CallbackFnc    MCallback;
   GuiProperties* _pcontainer;
};

#ifdef JENGINE_INLINE
#  include "guipropertybase.inl"
#endif

#endif
