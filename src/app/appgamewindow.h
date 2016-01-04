#pragma once

#include <agile.h>
#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <map>

#include "core/window/gamewindow.h"

ref class WindowEventHandler;

class AppInputDevice;

class AppGameWindow : public GameWindow
{
public:
   AppGameWindow();

   AppInputDevice& getInputDevice() {
      return *mpInput;
   }

   void setInputDevice(AppInputDevice& input) {
      mpInput = &input;
   }

   // query
   virtual int getHandle() const override;
   virtual int getWidth() const override;
   virtual int getHeight() const override;

   // overloads
   virtual bool initDevice(Graphics::Device& device) override;
   virtual void resize(int width, int height) override;
   virtual void toggleFullscreen() override;
   virtual void update() override;
   virtual void display() override;

protected:

   virtual bool doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen) override;
   virtual void doDestroy() override;

private:

 // operations
   void setWindow(Windows::UI::Core::CoreWindow^ window);
   void updateRenderTargetSize();
   DXGI_MODE_ROTATION computeDisplayRotation();
   
 // data members
   Platform::Agile<Windows::UI::Core::CoreWindow> mWindow;
   WindowEventHandler^ mHandler;

   // d3d variables
   Microsoft::WRL::ComPtr<ID3D11Device3>        m_d3dDevice;
   Microsoft::WRL::ComPtr<ID3D11DeviceContext3>	m_d3dContext;
   Microsoft::WRL::ComPtr<IDXGISwapChain3>      m_swapChain;

   float	m_dpi;
   float m_effectiveDpi; // This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
   
   Windows::Foundation::Size						m_d3dRenderTargetSize;
   Windows::Foundation::Size						m_outputSize;
   Windows::Foundation::Size						m_logicalSize;

   Windows::Graphics::Display::DisplayOrientations	m_nativeOrientation;
   Windows::Graphics::Display::DisplayOrientations	m_currentOrientation;

   AppInputDevice* mpInput;
};

