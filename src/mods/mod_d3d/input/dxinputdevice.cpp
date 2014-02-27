
#include "dxinputdevice.h"

namespace Input
{

DXInputDevice::DXInputDevice():
   InputDevice(),
   mpInput(NULL),
   mpKeyboard(NULL),
   mKeyStates()
{
}

bool DXInputDevice::create(int windowhandle)
{
   HMODULE hDLL = GetModuleHandle(NULL);
   HRESULT hr = DirectInput8Create(hDLL, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mpInput, NULL);
   if ( FAILED(hr) )
   {
      return false;
   }

   hr = mpInput->CreateDevice(GUID_SysKeyboard, &mpKeyboard, NULL);
   if ( FAILED(hr) )
   {
      return false;
   }

   hr = mpKeyboard->SetCooperativeLevel((HWND)windowhandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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
   return false; //  (mKeyStates[Keys[key]] & 0x80) == 0x80;
}

} // namespace Input
