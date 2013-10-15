
#include "win32gamewindow.h"

#include <tchar.h>

#include "core/string/string.h"
#include "core/system/driver.h"

#include "win32deviceinfo.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   Win32GameWindow* pwindow = reinterpret_cast<Win32GameWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
   if ( pwindow != NULL )
   {
      return pwindow->handleMessage(message, wParam, lParam);
   }
   
   return DefWindowProc(hWnd, message, wParam, lParam);
}

Win32GameWindow::Win32GameWindow():
   GameWindow(),
   mWnd(NULL)
{
}

bool Win32GameWindow::doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen)
{
   HINSTANCE hInstance = GetModuleHandle(NULL);

   static TCHAR szWindowClass[] = _T("win32app");
   static TCHAR szTitle[] = _T("JEngine SSE");

   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = 0; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
   wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName   = NULL;
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = 0; //LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

   if ( !RegisterClassEx(&wcex) )
   {
      return false;
   }

   RECT client = { 0, 0, width, height };
   ::AdjustWindowRect(&client, WS_OVERLAPPEDWINDOW, false);

   mWnd = ::CreateWindow(
      szWindowClass,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      client.right - client.left, 
      client.bottom - client.top,
      NULL,
      NULL,
      hInstance,
      NULL
   );

   if ( mWnd == NULL )
   {
      DWORD error = GetLastError();
      return false;
   }

   mWidth = width;
   mHeight = height;

   ::SetWindowLongPtr(mWnd, GWLP_USERDATA, (LONG)this);

   ::ShowWindow(mWnd, SW_SHOW);
   ::UpdateWindow(mWnd);

   return true;
}

void Win32GameWindow::doDestroy()
{
   if ( mWnd != NULL )
   {
      ::DestroyWindow(mWnd);
   }
}

// - Query

int Win32GameWindow::getHandle() const
{
   return (int) mWnd;
}

int Win32GameWindow::getWidth() const
{
   return mWidth;
}

int Win32GameWindow::getHeight() const
{
   return mHeight;
}

Driver* Win32GameWindow::loadDriver()
{
#ifdef _DEBUG
   Driver* pdriver = doLoadDriver(UTEXT("D3Dd.dll"));
   pdriver->setDeviceInfo(new Win32DeviceInfo((uint32_t)mWnd));
   return pdriver;
#else
   return doLoadDriver(UTEXT("D3D.dll"));
#endif
}

void Win32GameWindow::resize(int width, int height)
{
}

void Win32GameWindow::toggleFullscreen()
{
}

void Win32GameWindow::update()
{
   MSG msg;
   while ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
}

void Win32GameWindow::display()
{
}

int Win32GameWindow::handleMessage(UINT message, UINT wparam, UINT lparam)
{
   switch ( message )
   {
   case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint(mWnd, &ps);
         EndPaint(mWnd, &ps);
      }
      break;

   case WM_CLOSE:
      {
         fireWindowClosed();
      }
      break;
   }

   return DefWindowProc(mWnd, message, wparam, lparam);
}
