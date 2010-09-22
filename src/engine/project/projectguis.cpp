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
#include "projectguis.h"

#include <tinyxml.h>

ProjectGUIs::ProjectGUIs():
   _files()
{
}

//////////////////////////////////////////////////////////////////////////
// - Storage interface
//////////////////////////////////////////////////////////////////////////

void ProjectGUIs::toXML(TiXmlElement& gui)
{
   for ( int index = 0; index < _files.size(); ++index )
   {
      TiXmlText* pfilename     = new TiXmlText(_files[index]);

      TiXmlElement* pfileblock = new TiXmlElement("file");
      pfileblock->LinkEndChild(pfilename);

      gui.LinkEndChild(pfileblock);
   }
}

bool ProjectGUIs::fromXML(TiXmlElement& gui)
{
   TiXmlElement* pfile = static_cast<TiXmlElement*>(gui.FirstChild("file"));
   while ( pfile != NULL )
   {
      TiXmlText* pfiletext = static_cast<TiXmlText*>(pfile->FirstChild());
      if ( pfiletext != NULL )
      {
         _files.push_back(pfiletext->Value());
      }

      pfile = static_cast<TiXmlElement*>(gui.IterateChildren("file", pfile));
   }

   return true;
}
