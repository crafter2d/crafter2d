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

#ifdef WIN32
HDC Movie::hdc = CreateCompatibleDC(0);
#endif

Movie::Movie()
#ifdef WIN32
 : mpStream(NULL),
   mpFramePointer(NULL),
   mDIB(NULL),
   mBitmap(NULL),
   mNumberOfFrames(0),
   mFramesPerSecond(0),
   mCurrentFrame(0),
   mUpdateRate(0.0f),
   mFrameTime(0.0f),
   mpData(0)
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
   if ( mpStream != NULL )
   {
      DeleteObject(mBitmap);
	   DrawDibClose(mDIB);

	   AVIStreamGetFrameClose(mpFramePointer);
	   AVIStreamRelease(mpStream);
      mpFramePointer = NULL;
      mpStream = NULL;

      mpData = NULL;
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
	else 
   {
      Log::getInstance().error("Movie.load: unknown movie format (%s)", file);
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
   Log& log = Log::getInstance();

   // try to open the avi file
   if ( AVIStreamOpenFromFile(&mpStream, filename, streamtypeVIDEO, 0, OF_READ, NULL) != 0 )
   {
      log.error("Can not open %s movie file.", filename);   
      return false;
   }
   else 
   {
      // get avi dimensions and length
      AVISTREAMINFO psi;
      AVIStreamInfo(mpStream, &psi, sizeof(psi));
      _width = psi.rcFrame.right-psi.rcFrame.left;
      _height = psi.rcFrame.bottom-psi.rcFrame.top;
      mNumberOfFrames = AVIStreamLength(mpStream);

      _actualwidth = findNextPowerOfTwo(_width);
      _actualheight = findNextPowerOfTwo(_height);

      mDIB = DrawDibOpen();

      // calculate how much frame per second must be displayed
      mFramesPerSecond = psi.dwRate / psi.dwScale;
      mUpdateRate = 1.0f / mFramesPerSecond;

      // create bitmap info
      BITMAPINFOHEADER  bmih;
      bmih.biSize = sizeof (BITMAPINFOHEADER);
	   bmih.biPlanes = 1;
	   bmih.biBitCount = 24;
      bmih.biWidth = _actualwidth;//getWidth();
      bmih.biHeight = _actualheight;// getHeight();
	   bmih.biCompression = BI_RGB;
	   mBitmap = CreateDIBSection (hdc, (BITMAPINFO*)(&bmih), DIB_RGB_COLORS, (void**)(&mpData), NULL, 0);
	   SelectObject (hdc, mBitmap);

      // get the AVI frame pointer
      mpFramePointer = AVIStreamGetFrameOpen(mpStream, NULL);
      if ( mpFramePointer == NULL )
      {
         log.error("Can not open the stream frame of %s.", filename);
         return false;
      }

      target = getRenderTarget ();

      glGenTextures(1, &tex);
	   glBindTexture(target, tex);
      update(0);
      
      return true;
   }
#endif
	return true;
}

void Movie::update(float delta)
{
#ifdef WIN32
   LPBITMAPINFOHEADER lpbi;

   // get the frame data
   lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(mpFramePointer, mCurrentFrame);
   uchar* pdata = (uchar*)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);

   // copy the data to our buffer
   DrawDibDraw (mDIB, hdc, 0, 0, _actualwidth, _actualheight, lpbi, pdata, 0, 0, getWidth(), getHeight(), 0);

   // update the texture (flipping r and b channel)
   glBindTexture (target, tex);
   glTexImage2D (target, 0, 3, _actualwidth, _actualheight, 0, GL_BGR, GL_UNSIGNED_BYTE, mpData);
   //glTexSubImage2D (target, 0, 0, 0, getWidth(), getHeight(), GL_BGR, GL_UNSIGNED_BYTE, data);

   // go to next frame
   mFrameTime += delta;
   if ( mFrameTime > mUpdateRate )
   {
	   if ( ++mCurrentFrame == mNumberOfFrames )
		   mCurrentFrame=0;

      mFrameTime = 0.0f;
	}
#endif
}
