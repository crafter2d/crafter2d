/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   crafter2d@outlook.com                                                 *
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

#include "core/modules/graphicsmodule.h"
#include "core/modules/inputmodule.h"
#include "core/modules/modulecollection.h"

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

using namespace c2d;

extern "C" OGL_API ModuleCollection* cdecl getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   GraphicsModule* pmodule = new GraphicsModule();
   pmodule->setDevice(new Graphics::OGLDevice());
   pmodules->add(pmodule);

   InputModule* pinputmodule = new InputModule();
   pinputmodule->setDevice(new OGLInputDevice());
   pmodules->add(pinputmodule);

   return pmodules;
}
