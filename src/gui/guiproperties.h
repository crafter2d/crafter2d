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
#ifndef GUIPROPERTIES_H
#define GUIPROPERTIES_H

#include <vector>

class GuiPropertyBase;
class TiXmlElement;

/**
@author Jeroen Broekhuizen
*/
class GuiProperties
{
public:
   typedef std::vector<GuiPropertyBase*> Properties;

    GuiProperties();
   ~GuiProperties();

   void  Add(GuiPropertyBase* pproperty);
   void  clear();

   int   Size() const;

   bool             hasOwner() const;
   void             setOwner(GuiWnd* powner);
   GuiWnd&          getOwner();

   GuiPropertyBase* find(const std::string& name);

   TiXmlElement*  toXML() const;
   void           fromXML(const TiXmlElement* pelement);

   const GuiPropertyBase& operator[](int index) const;
         GuiPropertyBase& operator[](int index);

private:
   Properties  MProperties;
   GuiWnd*     _powner;
};

#endif
