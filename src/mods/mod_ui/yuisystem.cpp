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
#include "yuisystem.h"

#include "yuiscriptregistration.h"

#include "core/vfs/filesystem.h"

#include "engine/client.h"

namespace c2d
{
   YuiSystem::YuiSystem():
      mpContentManager(nullptr),
      mpScriptManager(nullptr)
   {
   }

   bool YuiSystem::initialize(ContentManager& contentmgr, ScriptManager& scriptmgr, int width, int height)
   {
      mpContentManager = &contentmgr;
      mpScriptManager = &scriptmgr;

      return true;
   }

   void YuiSystem::update(Graphics::RenderContext & context, float delta)
   {
   }

   void YuiSystem::render(Graphics::RenderContext & context)
   {
   }

   void YuiSystem::setTheme(const String& themefile)
   {

   }

   c2d::YuiWindow* YuiSystem::load(const String & file)
   {
      return nullptr;
   }
}
