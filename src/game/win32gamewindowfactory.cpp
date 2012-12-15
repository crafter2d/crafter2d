
#include "win32gamewindowfactory.h"

#include "win32gamewindow.h"

GameWindow* Win32GameWindowFactory::createWindow()
{
   return new Win32GameWindow();
}
