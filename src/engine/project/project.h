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
#ifndef PROJECT_H
#define PROJECT_H

#include "projectguis.h"

class TiXmlElement;

/**
   @author Jeroen Broekhuizen <jeroen@jengine.homedns.org>
*/
class Project
{
public:
   Project();
   ~Project();
   
   bool  load(const std::string& projectfile);

private:
   bool  loadContents(TiXmlElement& project);
   bool  loadGUI(TiXmlElement& dialog);

   ProjectGUIs _guifiles;
};

#endif
