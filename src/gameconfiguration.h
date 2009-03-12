/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#ifndef GAME_CONFIGURATION_H_
#define GAME_CONFIGURATION_H_

#include <string>

class TiXmlElement;

class GameConfiguration
{
public:
   GameConfiguration();
   ~GameConfiguration();

 // get/set
   const std::string&   getName() const;

   const std::string&   getImagePath() const;
   const std::string&   getObjectPath() const;
   const std::string&   getScriptPath() const;
   const std::string&   getShaderPath() const;
   const std::string&   getWorldPath() const;

 // load interface
   bool load(const std::string& filename);

 // operations
   GameConfiguration*   clone() const;

private:
   GameConfiguration(const GameConfiguration& other);

   bool loadPathInfo(const TiXmlElement& game);

   std::string mName;
   std::string mDescription;
   std::string mRoot;
   std::string mImagePath;
   std::string mObjectPath;
   std::string mScriptPath;
   std::string mShaderPath;
   std::string mWorldPath;
};

#ifdef JENGINE_INLINE
#  include "gameconfiguration.inl"
#endif

#endif // GAME_CONFIGURATION_H_
