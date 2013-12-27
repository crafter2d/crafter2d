/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ogl_base.h"

#include <Windows.h>

#include "core/graphics/graphicssystem.h"
#include "core/input/inputsystem.h"
#include "core/modules/modulecollection.h"
#include "core/system/systemmodule.h"

#include "ogldevice.h"
#include "input/oglinputdevice.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
   switch (ul_reason_for_call)
   {
   case DLL_PROCESS_ATTACH:
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
   case DLL_PROCESS_DETACH:
      break;
   }
   return TRUE;
}

extern "C" OGL_API ModuleCollection* cdecl getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   c2d::SystemModule* pmodule = new c2d::SystemModule();
   pmodule->setSystem(new c2d::GraphicsSystem(new Graphics::OGLDevice()));
   pmodules->add(pmodule);

   pmodule = new c2d::SystemModule();
   pmodule->setSystem(new c2d::InputSystem(new OGLInputDevice()));
   pmodules->add(pmodule);

   return pmodules;
}
