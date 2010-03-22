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
#ifndef GUI_IMAGEBUTTON_H_
#define GUI_IMAGEBUTTON_H_

#include <string>

#include "gui/button/guibutton.h"

/// @author Jeroen Broekhuizen
/// \brief Implementation of an image based button
class GuiImageButton : public GuiButton
{
public:
   DESIGNER_REGISTRATION(GuiImageButton)

   static void onPressedImageChanged(GuiWnd* pwnd);

   GuiImageButton();

   void           setImage(const char* image);
   void           setTextureCoordinates(const GuiRect& rect);
   void           setOffset(const Vector& ofs);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);

   void  paintNormal(const GuiGraphics& graphics);
   void  paintPressed(const GuiGraphics& graphics);

   bool  loadImage(const char* file, Texture& texture);

private:
   DECLARE_PROPERTIES
   STR_PROPERTY(PressedImage)

   Texture  _normalTex;
   Texture  _pressedTex;
   GuiRect  texCoords;
};

#endif
