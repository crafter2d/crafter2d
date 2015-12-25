#pragma once

#include "core/window/gamewindowfactory.h"

class AppGameWindowFactory : public GameWindowFactory
{
public:
   virtual GameWindow* createWindow() override;
};

