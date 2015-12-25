
#ifndef WIN32_GAMEWINDOW_FACTORY_H
#define WIN32_GAMEWINDOW_FACTORY_H

#include "core/window/gamewindowfactory.h"

class Win32GameWindowFactory : public GameWindowFactory
{
public:
   virtual GameWindow* createWindow();
};

#endif // WIN32_GAMEWINDOW_FACTORY_H
