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
#include "core/defines.h"

namespace ui
{
   // - Get/set

   INLINE Graphics::Device& System::getDevice()
   {
      return mDevice;
   }

   INLINE Graphics::RenderContext& System::getContext()
   {
      return mContext;
   }

   INLINE LookAndFeel& System::getLookAndFeel()
   {
      ASSERT_PTR(mpLookAndFeel);
      return *mpLookAndFeel;
   }
   
   INLINE void System::setLookAndFeel(LookAndFeel* plookandfeel)
   {
      if ( mpLookAndFeel != plookandfeel )
      {
         mpLookAndFeel = plookandfeel;

         onLookAndFeelChanged();
      }
   }
}
