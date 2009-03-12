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
#include "movie.h"
#include "console.h"

#ifdef WIN32
HDC Movie::hdc = CreateCompatibleDC(0);
#endif

Movie::Movie()
#ifdef WIN32
 : pavi(0),
   pgf(0),
   lastframe(0),
   frame(0),
   next(0),
   mpf(0),
   hdd(0),
   hBitmap(0),
   data(0)
#endif
{
}

Movie::~Movie()
{
}

void Movie::release()
{
   Texture::release();
#ifdef WIN32
   if (pavi != 0) {
      DeleteObject(hBitmap);
	   DrawDibClose(hdd);
	   AVIStreamGetFrameClose(pgf);
	   AVIStreamRelease(pavi);
      pgf = 0;
      pavi = 0;
   }
#endif
}

bool Movie::load(const char* file)
{
#ifdef WIN32
	bool ok = false;
   filename = file;
   tex = 0;

   target = getRenderTarget ();
   if (strstr(file, "avi") != NULL)
      ok = loadAVI(file);
	else {
      Console::getInstance().printf("Movie.load: unknown movie format (%s)", file);
		return false;
	}
	
	glTexParameteri (target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return ok;
#endif
	return true;
}

bool Movie::loadAVI(const char* filename)
{
#ifdef WIN32
   Console& console = Console::getInstance();

   // try to open the avi file
   if (AVIStreamOpenFromFile(&pavi, filename, streamtypeVIDEO, 0, OF_READ, NULL) !=0) {
      console.printf("Can not open %s movie file.", filename);   
      return false;
   }
   else {
      // get avi dimensions and length
      AVISTREAMINFO psi;
      AVIStreamInfo(pavi, &psi, sizeof(psi));
      _width = psi.rcFrame.right-psi.rcFrame.left;
      _height = psi.rcFrame.bottom-psi.rcFrame.top;
      lastframe = AVIStreamLength(pavi);

      _actualwidth = findNextPowerOfTwo(_width);
      _actualheight = findNextPowerOfTwo(_height);

      hdd = DrawDibOpen();

      // calculate how much frame per second must be displayed
      mpf = psi.dwRate / psi.dwScale;

      // create bitmap info
      bmih.biSize = sizeof (BITMAPINFOHEADER);
	   bmih.biPlanes = 1;
	   bmih.biBitCount = 24;
      bmih.biWidth = _actualwidth;//getWidth();
      bmih.biHeight = _actualheight;// getHeight();
	   bmih.biCompression = BI_RGB;
	   hBitmap = CreateDIBSection (hdc, (BITMAPINFO*)(&bmih), DIB_RGB_COLORS, (void**)(&data), NULL, 0);
	   SelectObject (hdc, hBitmap);

      // get the AVI frame pointer
      pgf = AVIStreamGetFrameOpen(pavi, NULL);
      if (pgf == NULL) {
         console.printf("Can not open the stream frame of %s.", filename);
         return false;
      }

      target = getRenderTarget ();

      glGenTextures (1, &tex);
	   glBindTexture (target, tex);
      update(SDL_GetTicks());
      
      //Uint8* pdata = ensureProperSize(3, data, _width, _height);
      glTexImage2D (target, 0, 3, _actualwidth, _actualheight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

      //glTexImage2D (target, 0, 3, getWidth(), getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, data);
      return true;
   }
#endif
	return true;
}

void Movie::update(Uint32 tick)
{
#ifdef WIN32
   LPBITMAPINFOHEADER lpbi;

   // get the frame data
   lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);
   char* pdata = (char*)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);

   // copy the data to our buffer and flip the G and B bytes
   DrawDibDraw (hdd, hdc, 0, 0, _actualwidth, _actualheight, lpbi, pdata, 0, 0, getWidth(), getHeight(), 0);

   // update the texture
   glBindTexture (target, tex);
   glTexImage2D (target, 0, 3, _actualwidth, _actualheight, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
   //glTexSubImage2D (target, 0, 0, 0, getWidth(), getHeight(), GL_BGR, GL_UNSIGNED_BYTE, data);

   // go to next frame
   if (tick - next > mpf) {
	   if (++frame == lastframe)
		   frame=0;
		next = tick;
	}
#endif
}
