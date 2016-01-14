
#include "uisystem.h"

#include <memory>
#include <tuple>

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
   mpDevice = &device;
   mpScriptManager = &scriptmanager;
   return true;
}

// - Event notifications

void c2d::UiSystem::onKeyEvent(const Input::KeyEvent& event)
{
}

void c2d::UiSystem::onMouseEvent(const Input::MouseEvent& event)
{
}

