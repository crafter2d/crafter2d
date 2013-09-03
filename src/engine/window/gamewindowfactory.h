
#ifndef GAME_WINDOW_FACTORY_H_
#define GAME_WINDOW_FACTORY_H_

#include "core/system/platform.h"

class GameWindow;

class GameWindowFactory
{
public:
   virtual GameWindow* createWindow() = 0;
};

#endif // GAME_WINDOW_FACTORY_H_
