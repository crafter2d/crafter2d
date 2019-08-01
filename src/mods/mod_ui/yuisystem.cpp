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

#include "core/content/contentmanager.h"
#include "core/vfs/filesystem.h"

#include "yuitheme.h"
#include "yuiwindow.h"

namespace c2d
{
   YuiSystem::YuiSystem():
      mpContentManager(nullptr),
      mpScriptManager(nullptr),
      mpCurrentTheme(nullptr),
      mRenderer(),
      mWindows()
   {
   }

   bool YuiSystem::initialize(ContentManager& contentmgr, ScriptManager& scriptmgr, int width, int height)
   {
      mpContentManager = &contentmgr;
      mpScriptManager = &scriptmgr;

      mRenderer.create(contentmgr.getDevice());

      return true;
   }

   void YuiSystem::viewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport)
   {
      mRenderer.viewportChanged(context, viewport);
   }

   void YuiSystem::update(Graphics::RenderContext & context, float delta)
   {
   }

   void YuiSystem::render(Graphics::RenderContext & context)
   {
      if ( mpCurrentTheme )
      {
         mRenderer.beginRendering(context, mpCurrentTheme->getAtlas());

         for ( auto pwindow : mWindows )
         {
            pwindow->render(mRenderer);
         }

         mRenderer.endRendering();
      }
   }

   void YuiSystem::setTheme(const String& themefile)
   {
      mpCurrentTheme = mpContentManager->loadContent<YuiTheme>(themefile);
   }

   YuiWindow* YuiSystem::load(const String & file)
   {
      auto pwindow = std::make_unique<YuiWindow>(*this);
      if ( !pwindow->create() )
      {
         return nullptr;
      }

      mWindows.push_back(pwindow.release());
      return mWindows.back();
   }
}
