/***************************************************************************
*   Copyright (C) 2016 by Jeroen Broekhuizen                              *
*   crafter2d@outlook.com                                                 *
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
#include "yuiwindow.h"

#include "core/graphics/device.h"
#include "core/graphics/effect.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/utils.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/viewport.h"

#include "yuibatchrenderer.h"

using namespace Graphics;

namespace c2d
{
   YuiWindow::YuiWindow(YuiSystem& system) :
      mSystem(system)
   {
   }

   bool YuiWindow::create()
   {
      mWindowRect.top = 20;
      mWindowRect.left = 10;
      mWindowRect.bottom = 80;
      mWindowRect.right = 150;

      RectF leftcorner(mWindowRect.left, mWindowRect.top, 18, 18);
      RectF rightcorner(mWindowRect.right - 18, mWindowRect.top, 18, 18);

      mRenderList.reserve(7);
      mRenderList.push_back({ leftcorner, 0 });
      mRenderList.push_back({ rightcorner, 1 });
      mRenderList.emplace_back(RectF(mWindowRect.left, mWindowRect.bottom - 18, 18, 18), 2);
      mRenderList.emplace_back(RectF(rightcorner.left, mWindowRect.bottom - 18, 18, 18), 3);

      float sideborder_height = mWindowRect.height() - 36;
      mRenderList.emplace_back(RectF(leftcorner.right, mWindowRect.top, mWindowRect.width() - 36, mWindowRect.height()), 4);
      mRenderList.emplace_back(RectF(leftcorner.left, leftcorner.bottom, 18, sideborder_height), 4);
      mRenderList.emplace_back(RectF(rightcorner.left, rightcorner.bottom, 18, sideborder_height), 4);

      return true;
   }

   void YuiWindow::render(YuiBatchRenderer& renderer)
   {
      for ( auto& pair : mRenderList )
      {
         renderer.renderRect(pair.first, pair.second);
      }
   }

} // namespace c2d
