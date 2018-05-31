
#include "dxinputdevice.h"

#include "core/window/gamewindow.h"

namespace Input
{

DXInputDevice::DXInputDevice():
   InputDevice(),
   mpInput(nullptr),
   mpKeyboard(nullptr),
   mKeyStates()
{
}

bool DXInputDevice::create(GameWindow& window)
{
   HMODULE hDLL = GetModuleHandle(nullptr);
   HRESULT hr = DirectInput8Create(hDLL, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mpInput, nullptr);
   if ( FAILED(hr) )
   {
      return false;
   }

   hr = mpInput->CreateDevice(GUID_SysKeyboard, &mpKeyboard, nullptr);
   if ( FAILED(hr) )
   {
      return false;
   }

   hr = mpKeyboard->SetCooperativeLevel((HWND)window.getHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
   hr = mpKeyboard->SetDataFormat(&c_dfDIKeyboard);

   return true;
}

void DXInputDevice::update()
{
   HRESULT hr = mpKeyboard->GetDeviceState(sizeof(char) * 256, mKeyStates);
   if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
   {
      hr = mpKeyboard->Acquire();
      if ( SUCCEEDED(hr) )
      {
         mpKeyboard->GetDeviceState(sizeof(char) * 256, mKeyStates);
      }
   }
}

static int Keys[] =
{
   0,
   DIK_LEFT,
   DIK_RIGHT,
   DIK_UP,
   DIK_DOWN,
   DIK_SPACE,
   DIK_RETURN,
   DIK_HOME,
   DIK_END,
   DIK_PGUP,
   DIK_PGDN,
   DIK_TAB,
   DIK_CAPITAL,
   DIK_1,
   DIK_2,
   DIK_3,
   DIK_4,
   DIK_5,
   DIK_6,
   DIK_7,
   DIK_8,
   DIK_9,
};

bool DXInputDevice::isKeyDown(int key)
{
   return (mKeyStates[Keys[key]] & 0x80) == 0x80;
}

} // namespace Input
