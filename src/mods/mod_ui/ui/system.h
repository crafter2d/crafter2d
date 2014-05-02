/***************************************************************************
*   Copyright (C) 2013 by Jeroen Broekhuizen                              *
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
#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

#include "windowmanager.h"

namespace Graphics
{
   class Device;
   class RenderContext;
}

namespace ui
{
   class LookAndFeel;
   class Window;

   class System
   {
   public:
    // statics
      static System& createSystem(Graphics::Device& device);
      static void    deleteSystem();
      static System& getInstance();

    // get/set
      Graphics::Device&        getDevice();
      Graphics::RenderContext& getContext();

      WindowManager& getWindowManager();

      LookAndFeel& getLookAndFeel();
      void         setLookAndFeel(LookAndFeel* plookandfeel);

   private:
      typedef std::vector<Window*> Frames;

    // private constructor
      System(Graphics::Device& device);

    // notifications
      void onLookAndFeelChanged();

    // singleton instance
      static System* mpInstance;

    // data
      Graphics::Device&        mDevice;
      Graphics::RenderContext& mContext;

      WindowManager mWindowManager;
      LookAndFeel*  mpLookAndFeel;
   };
}

#ifdef C2D_INLINE
#  include "system.inl"
#endif

#endif // SYSTEM_H
