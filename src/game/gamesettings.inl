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
#include "core/defines.h"

INLINE int GameSettings::getWidth() const
{
   return mWidth;
}

INLINE int GameSettings::getHeight() const
{
   return mHeight;
}

INLINE int GameSettings::getBitDepth() const
{
   return mBitDepth;
}

INLINE bool GameSettings::getFullScreen() const
{
   return mFullScreen;
}

INLINE const Color& GameSettings::getClearColor() const
{
   return mClearColor;
}

INLINE const Color& GameSettings::getTextColor() const
{
   return mTextColor;
}

INLINE const Color& GameSettings::getWindowColor() const
{
   return mWindowColor;
}

INLINE const Color& GameSettings::getBorderColor() const
{
   return mBorderColor;
}

INLINE const std::string& GameSettings::getName() const
{
   return mName;
}

INLINE const std::string& GameSettings::getIP() const
{
   return mIP;
}

INLINE int GameSettings::getPort() const
{
   return mPort;
}
