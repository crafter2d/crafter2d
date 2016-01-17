
#include "uisystem.h"

#include "core/defines.h"

c2d::UiSystem::UiSystem():
   mpDevice(nullptr),
   mpScriptManager(nullptr)
{
}

c2d::UiSystem::~UiSystem()
{
}

bool c2d::UiSystem::initialize(ScriptManager& scriptmanager, Graphics::Device& device, int width, int height)
{
   C2D_UNUSED(width);
   C2D_UNUSED(height);
   
   mpDevice = &device;
   mpScriptManager = &scriptmanager;
   return true;
}

// - Event notifications

void c2d::UiSystem::onKeyEvent(const Input::KeyEvent& event)
{
   C2D_UNUSED(event);
}

void c2d::UiSystem::onMouseEvent(const Input::MouseEvent& event)
{
   C2D_UNUSED(event);
}

