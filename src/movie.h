/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <SDL/SDL.h>
#include "texture.h"

#ifdef WIN32
#include <vfw.h>
#endif

class Movie: public Texture
{
public:
                  Movie();
   virtual        ~Movie();

   virtual bool   load(const char* file);
   virtual void   release();
   virtual void   update(Uint32 tick);

protected:
   bool           loadAVI(const char* filename);

#ifdef WIN32
   PAVISTREAM pavi;
   PGETFRAME pgf;
   BITMAPINFOHEADER bmih;
   Uint32 lastframe, frame, next, mpf;

   HDRAWDIB hdd;
   HBITMAP hBitmap;
   static HDC hdc;
   unsigned char* data;
#endif
};

#endif
