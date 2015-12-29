
#include "appinputdevice.h"

#include "appgamewindow.h"

AppInputDevice::AppInputDevice():
   Input::InputDevice(),
   mPressed()
{
}

AppInputDevice::~AppInputDevice()
{
}

bool AppInputDevice::create(GameWindow& window)
{
   AppGameWindow& appwindow = static_cast<AppGameWindow&>(window);
   appwindow.setInputDevice(*this);
   return true;
}

void AppInputDevice::update()
{
}

bool AppInputDevice::isKeyDown(int key)
{
   return mPressed[key];
}