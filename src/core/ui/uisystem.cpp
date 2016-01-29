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
#include "uisystem.h"

#include "core/defines.h"

c2d::UiSystem::UiSystem():
   mpDevice(nullptr),
   mpScriptManager(nullptr)
{
}

c2d::UiSystem::~UiSystem()
{
}

bool c2d::UiSystem::initialize(ScriptManager& scriptmanager, Graphics::Device& device, float width, float height)
{
   C2D_UNUSED(width);
   C2D_UNUSED(height);
   
   mpDevice = &device;
   mpScriptManager = &scriptmanager;
   return true;
}

// - Event notifications

void c2d::UiSystem::onKeyEvent(const Input::KeyEvent& event)
{
   C2D_UNUSED(event);
}

void c2d::UiSystem::onMouseEvent(const Input::MouseEvent& event)
{
   C2D_UNUSED(event);
}

