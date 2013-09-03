
#ifndef WIN32_GAMEWINDOW_H
#define WIN32_GAMEWINDOW_H

#include "engine/window/gamewindow.h"

#include <windows.h>

class Win32GameWindow : public GameWindow
{
public:
   Win32GameWindow();

 // query
   virtual int getHandle() const;
   virtual int getWidth() const;
   virtual int getHeight() const;

   virtual Driver* loadDriver() override;

 // overloads
   virtual void resize(int width, int height);
   virtual void toggleFullscreen();
   virtual void update();
   virtual void display();

 // message handling
   int handleMessage(UINT message, UINT wparam, UINT lparam);

protected:
 // creation/destroy
   virtual bool doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen);
   virtual void doDestroy();

private:

   HWND mWnd;
   int  mWidth;
   int  mHeight;
};

#endif // WIN32_GAMEWINDOW_H
