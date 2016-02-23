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
#include "movie.h"

#include "core/log/log.h"
#include "core/math/math.h"

Movie::Movie():
   mFrameWidth(0),
   mFrameHeight(0),
   mWidth(0),
   mHeight(0),
   mNumberOfFrames(0),
   mFramesPerSecond(0),
   mCurrentFrame(0),
   mUpdateRate(0.0f),
   mFrameTime(0.0f),
   mpData(nullptr)
{
}

Movie::~Movie()
{
}

void Movie::release()
{
}

bool Movie::load(const char* file)
{
   bool ok = false;

	return ok;
}

bool Movie::loadAVI(const char* filename)
{
	return false;
}

void Movie::update(float delta)
{
}
