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

namespace c2d
{

   INLINE GameWindowFactory& Client::getWindowFactory()
   {
      ASSERT_PTR(mpWindowFactory);
      return *mpWindowFactory;
   }

   INLINE void Client::setWindowFactory(GameWindowFactory& factory)
   {
      mpWindowFactory = &factory;
   }

   INLINE bool Client::hasWindow() const
   {
      return mpWindow != nullptr;
   }

   INLINE GameWindow& Client::getWindow()
   {
      ASSERT_PTR(mpWindow);
      return *mpWindow;
   }

   INLINE Graphics::Device& Client::getDevice()
   {
      ASSERT_PTR(mpDevice);
      return *mpDevice;
   }

   INLINE Graphics::RenderContext& Client::getRenderContext()
   {
      ASSERT_PTR(mpRenderContext);
      return *mpRenderContext;
   }

   INLINE Input::InputDevice& Client::getInput()
   {
      ASSERT_PTR(mpInputDevice);
      return *mpInputDevice;
   }

}
