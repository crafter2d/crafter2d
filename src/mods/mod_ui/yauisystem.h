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
#ifndef YAUI_SYSTEM_H
#define YAUI_SYSTEM_H

#include "core/ui/uisystem.h"

namespace c2d
{
   // The Yet Another UI system
   class YaUiSystem : public UiSystem
   {
   public:

      virtual bool initialize(ScriptManager& scriptmanager, Graphics::Device& device, float width, float height) override;

      virtual void update(Graphics::RenderContext& context, float delta) override;
      virtual void render(Graphics::RenderContext& context) override;
   };
}

#endif // YAUI_SYSTEM_H

