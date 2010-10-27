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
#ifndef GAME_SETTINGS_H_
#define GAME_SETTINGS_H_

#include <string>

#include "core/math/color.h"

class TiXmlDocument;
class TiXmlElement;

class GameSettings
{
public:
   GameSettings();
   ~GameSettings();

 // get/set
   int                  getWidth() const;
   int                  getHeight() const;
   int                  getBitDepth() const;
   bool                 getFullScreen() const;
   const Color&         getClearColor() const;

   const Color&         getTextColor() const;
   const Color&         getWindowColor() const;
   const Color&         getBorderColor() const;

   const std::string&   getName() const;
   const std::string&   getIP() const;
   int                  getPort() const;

 // operations
   void  initialize();

private:
 // operations
   void        load(const std::string& filename);

   void        loadScreenSettings(TiXmlDocument& doc);
   void        loadGuiSettings(TiXmlDocument& doc);
   void        loadMultiplayerSettings(TiXmlDocument& doc);

 // helpers
   int         queryInt(TiXmlElement& element, const std::string& name, int value);
   bool        queryBool(TiXmlElement& element, const std::string& name, bool value);
   std::string queryString(TiXmlElement& element, const std::string& name, const std::string& value);
   Color       queryColor(TiXmlElement& element, const std::string& name, const Color& value);

 // members
   int         mWidth;
   int         mHeight;
   int         mBitDepth;
   bool        mFullScreen;
   Color       mClearColor;

   Color       mTextColor;
   Color       mWindowColor;
   Color       mBorderColor;

   std::string mName;
   std::string mIP;
   int         mPort;
};

#ifdef JENGINE_INLINE
#  include "gamesettings.inl"
#endif

#endif // GAME_SETTINGS_H_
