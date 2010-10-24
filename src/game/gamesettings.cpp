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
#include "gamesettings.h"
#ifndef JENGINE_INLINE
#  include "gamesettings.inl"
#endif

#include <tinyxml.h>

const std::string SETTINGS_FILE = "config.xml";

GameSettings::GameSettings():
   mWidth(0),
   mHeight(0),
   mBitDepth(0),
   mFullScreen(false),
   mClearColor(),
   mTextColor(),
   mWindowColor(),
   mBorderColor(),
   mName(),
   mIP(),
   mPort(0)
{
}

GameSettings::~GameSettings()
{
}

void GameSettings::initialize()
{
   load(SETTINGS_FILE);
}

//////////////////////////////////////////////////////////////////////////
// - loading
//////////////////////////////////////////////////////////////////////////

void GameSettings::load(const std::string& filename)
{
   TiXmlDocument doc;
   if ( doc.LoadFile(filename) )
   {
      loadScreenSettings(doc);
      loadGuiSettings(doc);
   }
}

void GameSettings::loadScreenSettings(TiXmlDocument& doc)
{
   TiXmlElement* pscreen = doc.FirstChildElement("screen");
   if ( pscreen == NULL )
      return;

   mWidth      = queryInt(*pscreen, "width", 800);
   mHeight     = queryInt(*pscreen, "height", 600);
   mBitDepth   = queryInt(*pscreen, "bitdepth", 32);
   mFullScreen = queryBool(*pscreen, "fullscreen", false);
   mClearColor = queryColor(*pscreen, "clearcolor", GuiColor::black());
}

void GameSettings::loadGuiSettings(TiXmlDocument& doc)
{
   TiXmlElement* pgui = doc.FirstChildElement("gui");

   mTextColor    = queryColor(*pgui, "textcolor", GuiColor::red());
   mWindowColor  = queryColor(*pgui, "windowcolor", GuiColor::blue());
   mBorderColor  = queryColor(*pgui, "bordercolor", GuiColor::blue());
}

void GameSettings::loadMultiplayerSettings(TiXmlDocument& doc)
{
   TiXmlElement* pmp = doc.FirstChildElement("multiplayer");

   mName = queryString(*pmp, "name", "player");
   mIP   = queryString(*pmp, "ip", "");
   mPort = queryInt(*pmp, "port", 6400);
}

//////////////////////////////////////////////////////////////////////////
// - helpers
//////////////////////////////////////////////////////////////////////////

int GameSettings::queryInt(TiXmlElement& element, const std::string& name, int defaultvalue)
{
   TiXmlElement* pchild = element.FirstChildElement(name);
   if ( pchild == NULL )
      return defaultvalue;

   std::string value = pchild->GetText();
   return atoi(value.c_str());
}

bool GameSettings::queryBool(TiXmlElement& element, const std::string& name, bool defaultvalue)
{
   TiXmlElement* pchild = element.FirstChildElement(name);
   if ( pchild == NULL )
      return defaultvalue;

   std::string value = pchild->GetText();
   return (value.compare("1") == 0)
       || (value.compare("true") == 0);
}

std::string GameSettings::queryString(TiXmlElement& element, const std::string& name, const std::string& defaultvalue)
{
   TiXmlElement* pchild = element.FirstChildElement(name);
   if ( pchild == NULL )
      return defaultvalue;

   return pchild->GetText();
}

GuiColor GameSettings::queryColor(TiXmlElement& element, const std::string& name, const GuiColor& defaultvalue)
{
   TiXmlElement* pchild = element.FirstChildElement(name);
   if ( pchild == NULL )
      return defaultvalue;

   std::string value = pchild->GetText();

   int redindex = value.find(',');
   ASSERT(redindex != value.npos);
   int blueindex = value.find(',', redindex + 1);
   ASSERT(blueindex != value.npos);

   std::string red   = value.substr(0, redindex);
   std::string green = value.substr(redindex+1, blueindex - redindex - 1);
   std::string blue  = value.substr(blueindex+1, value.length() - blueindex);

   return GuiColor(atoi(red.c_str()), atoi(green.c_str()), atoi(blue.c_str()), 255);
}
