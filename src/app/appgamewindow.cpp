
#include "appgamewindow.h"

#include <Windows.h>

#include "core/defines.h"
#include "core/graphics/device.h"
#include "core/input/key.h"
#include "core/input/keyevent.h"
#include "core/math/point.h"

#include "mods/mod_d3d12/d3d11device.h"

using namespace Microsoft::WRL;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Devices::Input;

namespace DX
{
   // Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
   inline float ConvertDipsToPixels(float dips, float dpi)
   {
      static const float dipsPerInch = 96.0f;
      return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
   }
}

namespace DisplayMetrics
{
   // High resolution displays can require a lot of GPU and battery power to render.
   // High resolution phones, for example, may suffer from poor battery life if
   // games attempt to render at 60 frames per second at full fidelity.
   // The decision to render at full fidelity across all platforms and form factors
   // should be deliberate.
   static const bool SupportHighResolutions = false;

   // The default thresholds that define a "high resolution" display. If the thresholds
   // are exceeded and SupportHighResolutions is false, the dimensions will be scaled
   // by 50%.
   static const float DpiThreshold = 192.0f;		// 200% of standard desktop display.
   static const float WidthThreshold = 1920.0f;	// 1080p width.
   static const float HeightThreshold = 1080.0f;	// 1080p height.
};

using namespace Input;

ref class WindowEventHandler sealed
{
   using WindowPtr = Platform::Agile<CoreWindow>;
   using KeyMap = std::map<VirtualKey, int>;

   AppGameWindow& mAppWindow;
   WindowPtr mWindow;
   KeyMap mKeyMap;
   uint32 mPointerId;
   bool mPointerInUse;
   c2d::Point mPointerPosition;
   c2d::Point mFirstPosition;
   int mCurrentKey;
   bool mJump;

internal:
   WindowEventHandler(AppGameWindow& appwindow, WindowPtr window):
      mAppWindow(appwindow),
      mWindow(window),
      mKeyMap(),
      mPointerId(0),
      mPointerInUse(false),
      mPointerPosition(),
      mFirstPosition(),
      mJump(false)
   {
      registerHandlers();

      mKeyMap.insert({ VirtualKey::S, Key::DOWN });
      mKeyMap.insert({ VirtualKey::W, Key::UP });
      mKeyMap.insert({ VirtualKey::A, Key::LEFT });
      mKeyMap.insert({ VirtualKey::D, Key::RIGHT });
      mKeyMap.insert({ VirtualKey::Space, Key::SPACE });
   }

   void registerHandlers()
   {
      mWindow->SizeChanged +=
         ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &WindowEventHandler::OnWindowSizeChanged);

      mWindow->VisibilityChanged +=
         ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &WindowEventHandler::OnVisibilityChanged);

      mWindow->Closed +=
         ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &WindowEventHandler::OnWindowClosed);

      mWindow->PointerPressed +=
         ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowEventHandler::OnPointerPressed);

      mWindow->PointerMoved +=
         ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowEventHandler::OnPointerMoved);

      mWindow->PointerReleased +=
         ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowEventHandler::OnPointerReleased);

      mWindow->KeyDown +=
         ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &WindowEventHandler::OnKeyDown);

      mWindow->KeyUp +=
         ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &WindowEventHandler::OnKeyUp);

      DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

      currentDisplayInformation->DpiChanged +=
         ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &WindowEventHandler::OnDpiChanged);

      currentDisplayInformation->OrientationChanged +=
         ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &WindowEventHandler::OnOrientationChanged);

      DisplayInformation::DisplayContentsInvalidated +=
         ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(this, &WindowEventHandler::OnDisplayContentsInvalidated);
   }

   // Window event handlers.
   void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args)
   {
      mAppWindow.resize(args->Size.Width, args->Size.Height);
   }

   void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
   {
   }

   void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args)
   {
   }

   // Mouse/touch input

   void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
   {
      auto device = args->CurrentPoint->PointerDevice;
      auto deviceType = device->PointerDeviceType;

      if ( deviceType != PointerDeviceType::Mouse )
      {
         c2d::Point position(DX::ConvertDipsToPixels(args->CurrentPoint->Position.X, mAppWindow.getDPI()), DX::ConvertDipsToPixels(args->CurrentPoint->Position.Y, mAppWindow.getDPI()));
         if ( position.x > 350 && position.y > 600 )
         {
            KeyEvent keyevent(Key::SPACE, KeyEvent::ePressed, 0);
            mAppWindow.handleEvent(keyevent);
            mJump = true;
         }
         else if ( !mPointerInUse )
         {
            mPointerId = args->CurrentPoint->PointerId;
            mPointerInUse = true;
            mPointerPosition = position;
            mFirstPosition = position;
         }
      }
   }

   void OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
   {
      uint32 pointerId = args->CurrentPoint->PointerId;
      if ( pointerId == mPointerId )
      {
         c2d::Point position = c2d::Point(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);
         c2d::Point delta = position - mFirstPosition;

         int key = -1;
         if ( delta.x > 16.0f )
         {  
            key = Key::RIGHT;
         }
         else if ( delta.x < -16.0f )
         {
            key = Key::LEFT;
         }

         if ( key != -1 )
         {
            if ( key != mCurrentKey )
            {
               KeyEvent keyevent(key, KeyEvent::eReleased, 0);
               mAppWindow.handleEvent(keyevent);
            }

            mCurrentKey = key;
            KeyEvent keyevent(mCurrentKey, KeyEvent::ePressed, 0);
            mAppWindow.handleEvent(keyevent);
         }
      }
   }

   void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
   {
      uint32 pointerId = args->CurrentPoint->PointerId;
      c2d::Point position(DX::ConvertDipsToPixels(args->CurrentPoint->Position.X, mAppWindow.getDPI()), DX::ConvertDipsToPixels(args->CurrentPoint->Position.Y, mAppWindow.getDPI()));
      if ( position.x > 350 && position.y > 600 && mJump )
      {
         KeyEvent keyevent(Key::SPACE, KeyEvent::eReleased, 0);
         mAppWindow.handleEvent(keyevent);

         mJump = false;
      }
      
      if ( pointerId == mPointerId )
      {
         mPointerInUse = false;
         mPointerId = 0;

         KeyEvent keyevent(mCurrentKey, KeyEvent::eReleased, 0);
         mAppWindow.handleEvent(keyevent);
      }
   }

   // Input event handlers
   void OnKeyUp(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
   {
      auto it = mKeyMap.find(args->VirtualKey);
      if ( it != mKeyMap.end() )
      {
         // sender->GetKeyState(VirtualKey::Control).Value
         int modifiers = 0; // getModifiers();

         KeyEvent keyevent(it->second, KeyEvent::eReleased, modifiers);
         mAppWindow.handleEvent(keyevent);
      }
   }

   void OnKeyDown(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
   {
      using namespace Input;

      auto it = mKeyMap.find(args->VirtualKey);
      if ( it != mKeyMap.end() )
      {
         // sender->GetKeyState(VirtualKey::Control).Value
         int modifiers = 0; // getModifiers();

         KeyEvent keyevent(it->second, KeyEvent::ePressed, modifiers);
         mAppWindow.handleEvent(keyevent);
      }
   }

   // DisplayInformation event handlers.
   void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
   {
   }

   void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
   {
   }

   void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args)
   {
   }
};

AppGameWindow::AppGameWindow() :
   GameWindow(),
   mWindow(),
   mHandler()
{
}

AppGameWindow::~AppGameWindow()
{
}

bool AppGameWindow::doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen)
{
   setWindow(CoreWindow::GetForCurrentThread());

   mHandler = ref new WindowEventHandler(*this, mWindow);

   return true;
}

void AppGameWindow::doDestroy()
{
   mHandler = nullptr;
}

void AppGameWindow::handleEvent(const Input::KeyEvent& event)
{
   dispatch(event);
}

int AppGameWindow::getHandle() const
{
   return 0;
}

int AppGameWindow::getWidth() const
{
   return m_outputSize.Width;
}

int AppGameWindow::getHeight() const
{
   return m_outputSize.Height;
}

// overloads

bool AppGameWindow::initDevice(Graphics::Device& device)
{
   UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
   // If the project is in a debug build, enable debugging via SDK Layers with this flag.
   // creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

   D3D_FEATURE_LEVEL requestedlevel[] = {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3,
   };

   D3D_FEATURE_LEVEL level;
   ComPtr<ID3D11Device> d3ddevice;
   ComPtr<ID3D11DeviceContext> d3dcontext;

   HRESULT hr = D3D11CreateDevice(
      nullptr,					// Specify nullptr to use the default adapter.
      D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
      0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
      creationFlags,				// Set debug and Direct2D compatibility flags.
      requestedlevel,				// List of feature levels this app can support.
      ARRAYSIZE(requestedlevel),	// Size of the list above.
      D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps.
      &d3ddevice,					// Returns the Direct3D device created.
      &level,			// Returns feature level of device created.
      &d3dcontext					// Returns the device immediate context.
      );

   if ( FAILED(hr) )
   {
      // If the initialization fails, fall back to the WARP device.
      // For more information on WARP, see: 
      // http://go.microsoft.com/fwlink/?LinkId=286690
      hr = D3D11CreateDevice(
         nullptr,
         D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
         0,
         creationFlags,
         requestedlevel,
         ARRAYSIZE(requestedlevel),
         D3D11_SDK_VERSION,
         &d3ddevice,
         &level,
         &d3dcontext
         );
      if ( FAILED(hr) )
      {
         return false;
      }
   }

   d3ddevice.As(&m_d3dDevice);
   d3dcontext.As(&m_d3dContext);

   createWindowSizeDependentResources();

   auto pd3ddevice = dynamic_cast<Graphics::D3D11Device*>(&device);
   ASSERT_PTR(pd3ddevice);

   pd3ddevice->set(m_d3dDevice.Get(), m_d3dContext.Get(), m_swapChain.Get(), m_effectiveDpi);

   return true;
}

void AppGameWindow::resize(int width, int height)
{
   Windows::Foundation::Size size(width, height);
   if ( m_logicalSize != size )
   {
      m_logicalSize = size;

      createWindowSizeDependentResources();

      fireWindowResized();
   }
}

void AppGameWindow::toggleFullscreen()
{
}

void AppGameWindow::update()
{
   mWindow->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
}

void AppGameWindow::display()
{
}

// - Operations

void AppGameWindow::setWindow(CoreWindow^ window)
{
   mWindow = window;

   DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

   m_logicalSize = Windows::Foundation::Size(mWindow->Bounds.Width, mWindow->Bounds.Height);
   m_nativeOrientation = currentDisplayInformation->NativeOrientation;
   m_currentOrientation = currentDisplayInformation->CurrentOrientation;
   m_dpi = currentDisplayInformation->LogicalDpi;
}

bool AppGameWindow::createWindowSizeDependentResources()
{
   updateRenderTargetSize();

   // The width and height of the swap chain must be based on the window's
   // natively-oriented width and height. If the window is not in the native
   // orientation, the dimensions must be reversed.
   DXGI_MODE_ROTATION displayRotation = computeDisplayRotation();

   bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
   m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
   m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;

   if ( m_swapChain != nullptr )
   {
      
   }
   else
   {
      // Otherwise, create a new one using the same adapter as the existing Direct3D device.
      DXGI_SCALING scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
      DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

      swapChainDesc.Width = lround(m_d3dRenderTargetSize.Width);		// Match the size of the window.
      swapChainDesc.Height = lround(m_d3dRenderTargetSize.Height);
      swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;				// This is the most common swap chain format.
      swapChainDesc.Stereo = false;
      swapChainDesc.SampleDesc.Count = 1;								// Don't use multi-sampling.
      swapChainDesc.SampleDesc.Quality = 0;
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swapChainDesc.BufferCount = 2;									// Use double-buffering to minimize latency.
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// All Windows Store apps must use this SwapEffect.
      swapChainDesc.Flags = 0;
      swapChainDesc.Scaling = scaling;
      swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

      // This sequence obtains the DXGI factory that was used to create the Direct3D device above.
      ComPtr<IDXGIDevice3> dxgiDevice;
      m_d3dDevice.As(&dxgiDevice);

      ComPtr<IDXGIAdapter> dxgiAdapter;
      dxgiDevice->GetAdapter(&dxgiAdapter);

      ComPtr<IDXGIFactory4> dxgiFactory;
      dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

      ComPtr<IDXGISwapChain1> swapChain;
      HRESULT hr = dxgiFactory->CreateSwapChainForCoreWindow(
         m_d3dDevice.Get(),
         reinterpret_cast<IUnknown*>(mWindow.Get()),
         &swapChainDesc,
         nullptr,
         &swapChain
         );
      if ( FAILED(hr) )
      {
         return false;
      }

      swapChain.As(&m_swapChain);

      // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
      // ensures that the application will only render after each VSync, minimizing power consumption.
      dxgiDevice->SetMaximumFrameLatency(1);
   }

   m_swapChain->SetRotation(displayRotation);

   return true;
}

// Determine the dimensions of the render target and whether it will be scaled down.
void AppGameWindow::updateRenderTargetSize()
{
   m_effectiveDpi = m_dpi;

   // To improve battery life on high resolution devices, render to a smaller render target
   // and allow the GPU to scale the output when it is presented.
   if ( !DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold )
   {
      float width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_dpi);
      float height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_dpi);

      if ( width > DisplayMetrics::WidthThreshold && height > DisplayMetrics::HeightThreshold )
      {
         // To scale the app we change the effective DPI. Logical size does not change.
         m_effectiveDpi /= 2.0f;
      }
   }

   // Calculate the necessary render target size in pixels.
   m_outputSize.Width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_effectiveDpi);
   m_outputSize.Height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_effectiveDpi);

   // Prevent zero size DirectX content from being created.
   m_outputSize.Width = max(m_outputSize.Width, 1);
   m_outputSize.Height = max(m_outputSize.Height, 1);
}

// This method determines the rotation between the display device's native orientation and the
// current display orientation.
DXGI_MODE_ROTATION AppGameWindow::computeDisplayRotation()
{
   DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

   // Note: NativeOrientation can only be Landscape or Portrait even though
   // the DisplayOrientations enum has other values.
   switch ( m_nativeOrientation )
   {
   case DisplayOrientations::Landscape:
      switch ( m_currentOrientation )
      {
      case DisplayOrientations::Landscape:
         rotation = DXGI_MODE_ROTATION_IDENTITY;
         break;

      case DisplayOrientations::Portrait:
         rotation = DXGI_MODE_ROTATION_ROTATE270;
         break;

      case DisplayOrientations::LandscapeFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE180;
         break;

      case DisplayOrientations::PortraitFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE90;
         break;
      }
      break;

   case DisplayOrientations::Portrait:
      switch ( m_currentOrientation )
      {
      case DisplayOrientations::Landscape:
         rotation = DXGI_MODE_ROTATION_ROTATE90;
         break;

      case DisplayOrientations::Portrait:
         rotation = DXGI_MODE_ROTATION_IDENTITY;
         break;

      case DisplayOrientations::LandscapeFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE270;
         break;

      case DisplayOrientations::PortraitFlipped:
         rotation = DXGI_MODE_ROTATION_ROTATE180;
         break;
      }
      break;
   }
   return rotation;
}
