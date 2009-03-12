/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "../defines.h"

const GuiControl& GuiDesignDecorator::control() const
{
   ASSERT_PTR(MPControl)
   return *MPControl;
}

GuiControl& GuiDesignDecorator::control()
{
   return const_cast<GuiControl&>(me().control());
}

INLINE bool GuiDesignDecorator::selected() const
{
   return _selectionCtrl->getVisible();
}

INLINE GuiDesignSelection& GuiDesignDecorator::selectionCtrl()
{
   ASSERT_PTR(_selectionCtrl)
   return *_selectionCtrl;
}

//////////////////////////////////////////////////////////////////////////
// - Self
//////////////////////////////////////////////////////////////////////////

INLINE const GuiDesignDecorator& GuiDesignDecorator::me()
{
   return static_cast<const GuiDesignDecorator&>(*this);
}
