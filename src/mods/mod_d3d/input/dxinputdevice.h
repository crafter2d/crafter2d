
#ifndef DX_INPUT_DEVICE_H
#define DX_INPUT_DEVICE_H

#define DIRECTINPUT_VERSION 0x0800
#include <initguid.h>
#include <dinput.h>

#include "core/input/input.h"

namespace Input
{
   class DXInputDevice : public InputDevice
   {
   public:
      DXInputDevice();

    // overrides
      virtual bool create(GameWindow& window) override;
      virtual void update() override;

      virtual bool isKeyDown(int key);

   private:
    // data
      IDirectInput8*       mpInput;
      IDirectInputDevice8* mpKeyboard;
      BYTE                 mKeyStates[256];
   };
}

#endif // DX_INPUT_DEVICE_H
