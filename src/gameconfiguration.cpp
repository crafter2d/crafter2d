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

#include "gameconfiguration.h"
#ifndef JENGINE_INLINE
#  include "gameconfiguration.inl"
#endif

#include <tinyxml.h>

#include "console.h"

GameConfiguration::GameConfiguration():
   mName(),
   mDescription(),
   mRoot(".."),
   mImagePath(),
   mObjectPath(),
   mScriptPath(),
   mShaderPath(),
   mWorldPath()
{
}

GameConfiguration::GameConfiguration(const GameConfiguration& other):
   mName(other.mName),
   mDescription(other.mDescription),
   mRoot(other.mRoot),
   mImagePath(other.mImagePath),
   mObjectPath(other.mObjectPath),
   mScriptPath(other.mScriptPath),
   mShaderPath(other.mShaderPath),
   mWorldPath(other.mWorldPath)
{
}

GameConfiguration::~GameConfiguration()
{
}

//////////////////////////////////////////////////////////////////////////
// - Load interface
//////////////////////////////////////////////////////////////////////////

bool GameConfiguration::load(const std::string& filename)
{
   TiXmlDocument doc;
   bool success = false;

   if ( doc.LoadFile(filename) )
   {
      TiXmlElement* pgameelement = doc.FirstChildElement("game");
      if ( pgameelement == NULL )
      {
         Console::getInstance().error("missing GAME element in %s", filename.c_str());
         return false;
      }

      const char* pname = pgameelement->Attribute("name");
      if ( pgameelement == NULL )
      {
         Console::getInstance().error("game configuration file %s has no name", filename.c_str());
         return false;
      }
      else
      {
         mName = pname;
      }

      const TiXmlElement* pdescription = pgameelement->FirstChildElement("description");
      if ( pdescription != NULL )
         mDescription = pdescription->GetText();

      success = loadPathInfo(*pgameelement);
   }

   return success;
}

bool GameConfiguration::loadPathInfo(const TiXmlElement& game)
{
   std::string image  = "images";
   std::string object = "objects";
   std::string script = "scripts";
   std::string shader = "shaders";
   std::string world  = "worlds";

   const TiXmlElement* ppathselement = game.FirstChildElement("paths");
   if (  ppathselement != NULL )
   {
      const char* proot = ppathselement->Attribute("root");
      if ( proot != NULL )
         mRoot = proot;

      const TiXmlElement* pimagepath = ppathselement->FirstChildElement("imagepath");
      if ( pimagepath != NULL )
         image = pimagepath->GetText();

      const TiXmlElement* pobjectpath = ppathselement->FirstChildElement("objectpath");
      if ( pobjectpath != NULL )
         object = pobjectpath->GetText();

      const TiXmlElement* pscriptpath = ppathselement->FirstChildElement("scriptpath");
      if ( pscriptpath != NULL )
         script = pscriptpath->GetText();

      const TiXmlElement* pshaderpath = ppathselement->FirstChildElement("shaderpath");
      if ( pshaderpath != NULL )
         shader = pshaderpath->GetText();

      const TiXmlElement* pworldpath = ppathselement->FirstChildElement("worldpath");
      if ( pworldpath != NULL )
         world = pworldpath->GetText();
   }
   else
   {
      Console::getInstance().warning("game configuration %s has no path information, using default paths", mName.c_str());
   }

   mImagePath  = mRoot + "/" + image + "/";
   mObjectPath = mRoot + "/" + object + "/";
   mScriptPath = mRoot + "/" + script + "/";
   mShaderPath = mRoot + "/" + shader + "/";
   mWorldPath  = mRoot + "/" + world + "/";

   return true;
}

GameConfiguration* GameConfiguration::clone() const
{
   GameConfiguration* pclone = new GameConfiguration(*this);
   return pclone;
}
