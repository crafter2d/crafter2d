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
#ifndef _MOVIE_H_
#define _MOVIE_H_

#include "core/defines.h"

class Movie
{
public:
                  Movie();
   virtual        ~Movie();

   virtual bool   load(const char* file);
   virtual void   release();
   virtual void   update(float delta);

protected:
   bool           loadAVI(const char* filename);

   uint     mFrameWidth;
   uint     mFrameHeight;
   uint     mWidth;
   uint     mHeight;
   uint     mNumberOfFrames;
   uint     mFramesPerSecond;
   uint     mCurrentFrame;
   float    mUpdateRate;
   float    mFrameTime;
   uchar*   mpData;
};

#endif
