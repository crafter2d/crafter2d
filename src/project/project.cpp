/***************************************************************************
 *   Copyright (C) 2007 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "project.h"

#include <tinyxml.h>

Project::Project():
   _guifiles()
{
}

Project::~Project()
{
}

bool Project::load(const std::string& projectfile)
{
   TiXmlDocument doc;

   if ( !doc.LoadFile(projectfile) )
   {
   }
   else
   {
      TiXmlElement* pcontentSection = (TiXmlElement*)doc.FirstChild("project");
      if ( pcontentSection == NULL )
         return false;

      loadContents(*pcontentSection);
   }

   return true;
}

bool Project::loadContents(TiXmlElement& project)
{
   TiXmlElement* pcontentSection = (TiXmlElement*)project.FirstChild("contents");
   if ( pcontentSection == NULL )
      return false;
   else
      return loadGUI(*pcontentSection);
}

bool Project::loadGUI(TiXmlElement& contents)
{
   TiXmlElement* pguisection = (TiXmlElement*)contents.FirstChild("GUI");
   if ( pguisection == NULL )
      return false;

   return _guifiles.fromXML(*pguisection);
}
