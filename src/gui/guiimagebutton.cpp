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
#include "../console.h"
#include "../defines.h"

#include "guidesigner.h"

#include "guiimagebutton.h"

REGISTER_DESIGNER(GuiImageButton, GuiImageButtonId, "Image Button", 40, 15, 264)

IMPLEMENT_PROPERTIES(GuiImageButton, GuiButton)
   CONSTRUCT_STR_PROPERTY_WITH_CALLBACK(PressedImage, "Pressed image", "", onPressedImageChanged);
}

void GuiImageButton::onPressedImageChanged(GuiWnd* pwnd)
{
   GuiImageButton* pbutton = dynamic_cast<GuiImageButton*>(pwnd);
   pbutton->loadImage(pbutton->getPressedImage().c_str(), pbutton->_pressedTex);
}

GuiImageButton::GuiImageButton():
   GuiButton(),
   INIT_PROPERTY(PressedImage),
   _normalTex(),
   _pressedTex(),
   texCoords()
{
}

//////////////////////////////////////////////////////////////////////////
// - Creation interface
//////////////////////////////////////////////////////////////////////////

void GuiImageButton::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiButton::onCreate(rect, caption, style, parent);

   loadImage(getPressedImage().c_str(), _pressedTex);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiImageButton::paint(Uint32 tick, const GuiGraphics& graphics)
{
   if ( IS_SET(m_style, GUI_PRESSED) )
      paintPressed(graphics);
   else
      paintNormal(graphics);
}

void GuiImageButton::paintNormal(const GuiGraphics& graphics)
{
   if (_normalTex.isValid())
         graphics.drawImage(_normalTex, texCoords, m_frameRect);
}

void GuiImageButton::paintPressed(const GuiGraphics& graphics)
{
   if ( _pressedTex.isValid() )
      graphics.drawImage(_pressedTex, texCoords, m_frameRect);
   else
      paintNormal(graphics);
}

//////////////////////////////////////////////////////////////////////////
// - Operations interface
//////////////////////////////////////////////////////////////////////////

void GuiImageButton::setImage(const char* image)
{
   loadImage(image, _normalTex);
}

void GuiImageButton::setTextureCoordinates(const GuiRect& rect)
{
   if ( !_normalTex.isValid() )
      return;

   texCoords.left((rect.left() / _normalTex.getWidth()) * _normalTex.getSourceWidth());
   texCoords.right((rect.right() / _normalTex.getWidth()) * _normalTex.getSourceWidth());
   texCoords.top((rect.top() / _normalTex.getHeight()) * _normalTex.getSourceHeight());
   texCoords.bottom((rect.bottom() / _normalTex.getHeight()) * _normalTex.getSourceHeight());
}

void GuiImageButton::setOffset(const Vector& ofs)
{
   if ( _normalTex.isValid() )
   {
      if (ofs.x == 0 && ofs.y == 0)
         texCoords.set(0, _normalTex.getSourceWidth(), 0, _normalTex.getSourceHeight());
      else
         texCoords.offset(ofs.x, ofs.y);
   }
}

bool GuiImageButton::loadImage(const char* file, Texture& texture)
{
   texture.release();
   if ( file != NULL && strlen(file) > 0 )
   {
      if ( !texture.load(file) )
      {
         Console::getInstance().printf("Image %s not found.", file);
         return false;
      }

      texCoords.set(0, texture.getSourceWidth(), 0, texture.getSourceHeight());
   }

   return texture.isValid();
}
