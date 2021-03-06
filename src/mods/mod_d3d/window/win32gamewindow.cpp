
#include "win32gamewindow.h"

#include <tchar.h>

#include "core/string/string.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   Win32GameWindow* pwindow = reinterpret_cast<Win32GameWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
   if ( pwindow != nullptr )
   {
      return pwindow->handleMessage(message, wParam, lParam);
   }
   
   return DefWindowProc(hWnd, message, wParam, lParam);
}

Win32GameWindow::Win32GameWindow():
   GameWindow(),
   mWnd(nullptr),
   mWidth(0),
   mHeight(0)
{
}

bool Win32GameWindow::doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen)
{
   HINSTANCE hInstance = GetModuleHandle(nullptr);

   static TCHAR szWindowClass[] = _T("win32app");
   static TCHAR szTitle[] = _T("Crafter 2D");

   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = 0; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
   wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName   = nullptr;
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
	  nullptr,
	  nullptr,
      hInstance,
	  nullptr
   );

   if ( mWnd == nullptr )
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
   if ( mWnd != nullptr)
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

void Win32GameWindow::resize(int width, int height)
{
}

void Win32GameWindow::toggleFullscreen()
{
}

void Win32GameWindow::update()
{
   MSG msg;
   while ( ::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
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
