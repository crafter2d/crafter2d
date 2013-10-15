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
#ifndef DRIVER_H
#define DRIVER_H

#include "core/core_base.h"

namespace Graphics
{
   class Device;
   class IDeviceInfo;
}

namespace Input
{
   class InputDevice;
}

class CORE_API Driver
{
public:
   Driver();

 // get/set
   bool                    hasDeviceInfo() const;
   Graphics::IDeviceInfo&  getDeviceInfo();
   void                    setDeviceInfo(Graphics::IDeviceInfo* pinfo);

 // interface
   virtual Graphics::Device* createGraphicsDevice() = 0;
   virtual Input::InputDevice* createInputDevice() = 0;


private:

 // data
   Graphics::IDeviceInfo* mpInfo;
};

#endif // DRIVER_H
