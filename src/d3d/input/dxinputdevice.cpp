
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

bool DXInputDevice::isKeyDown(int key)
{
   return (mKeyStates[key] & 0x80) == 0x80;
}

} // namespace Input
