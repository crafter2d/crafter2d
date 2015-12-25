
#include "appgamewindowfactory.h"

#include "appgamewindow.h"

GameWindow* AppGameWindowFactory::createWindow()
{
   return new AppGameWindow();
}
