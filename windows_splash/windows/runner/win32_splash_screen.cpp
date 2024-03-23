// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <windows.h>
#include "win32_splash_screen.h"
#include "win32_image_loader.h"
#include "resource.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
constexpr const wchar_t kWindowClassName[] = L"SplashWindow";

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------

Win32SplashScreen::~Win32SplashScreen()
{
   Destroy();
}

bool Win32SplashScreen::Create(const Win32Window& owner, int minimumHideDelayTime)
{
   _minimumHideDelayTime = minimumHideDelayTime;
   const wchar_t* wndClass = GetWindowClass();

   // Create the window
   auto window = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, wndClass, L"", WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandle(nullptr), this);

   if (!window)
   {
      return false;
   }

   // Show the window, on top of the owner window
   RECT ownerRect{};
   GetWindowRect(owner.GetHandle(), &ownerRect);
   LoadSplashImage(window, POINT{ ownerRect.left, ownerRect.top });

   return true;
}

// static
LRESULT CALLBACK Win32SplashScreen::WndProc(HWND const hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept
{
   if (message == WM_NCCREATE)
   {
      auto wndStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
      SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndStruct->lpCreateParams));

      auto that = static_cast<Win32SplashScreen*>(wndStruct->lpCreateParams);
      EnableNonClientDpiScaling(hWnd);
      that->_hWindow = hWnd;
   }
   else if (Win32SplashScreen* that = GetThisFromHandle(hWnd))
   {
      return that->MessageHandler(hWnd, message, wParam, lParam);
   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Win32SplashScreen::MessageHandler(HWND hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept
{
   switch (message)
   {
      case WM_TIMER:
         KillTimer(hWnd, 1);
         DestroyWindow(_hWindow);
         return 0;

   case WM_DESTROY:
      _hWindow = nullptr;
      UnregisterWindowClass();
      return 0;

   case WM_DPICHANGED:
      {
         auto newRectSize = reinterpret_cast<RECT*>(lParam);
         LONG newWidth = newRectSize->right - newRectSize->left;
         LONG newHeight = newRectSize->bottom - newRectSize->top;

         SetWindowPos(hWnd, nullptr, newRectSize->left, newRectSize->top, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);

         return 0;
      }
   }

   return DefWindowProc(_hWindow, message, wParam, lParam);
}

void Win32SplashScreen::LoadSplashImage(HWND windows, POINT origin)
{
   ImageLoader loader;
   HBITMAP hBitmap = loader.Load(MAKEINTRESOURCE(IDB_SPLASH_SCREEN));
   if (hBitmap)
   {
      POINT ptZero { 0 };
      BITMAP bitmap{};
      GetObject(hBitmap, sizeof(BITMAP), &bitmap);
      SIZE sizeSplash { bitmap.bmWidth, bitmap.bmHeight };

      HDC hdcScreen = GetDC(nullptr);
      HDC hdcMem = CreateCompatibleDC(hdcScreen);
      auto hbmOld = SelectObject(hdcMem, hBitmap);

      BLENDFUNCTION blendFunction{};
      blendFunction.BlendOp = AC_SRC_OVER;
      blendFunction.SourceConstantAlpha = 255;
      blendFunction.AlphaFormat = AC_SRC_ALPHA;
      blendFunction.BlendFlags = 0;

#if true 
      UpdateLayeredWindow(windows, hdcScreen, &origin, &sizeSplash, hdcMem, &ptZero, RGB(0, 0, 0), &blendFunction, ULW_ALPHA);
#else
      ShowWindow(windows, SW_SHOWNOACTIVATE);
      SetWindowPos(windows, HWND_TOPMOST, origin.x, origin.y, sizeSplash.cx, sizeSplash.cy, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOOWNERZORDER | SWP_NOZORDER);
      BitBlt(hdcScreen, origin.x, origin.y, sizeSplash.cx, sizeSplash.cy, hdcMem, 0, 0, SRCCOPY);
#endif

      SelectObject(hdcMem, hbmOld);
      DeleteDC(hdcMem);
      ReleaseDC(nullptr, hdcScreen);
      DeleteObject(hBitmap);
   }
}

   // Returns the name of the window class, registering the class if it hasn't previously been registered.
const wchar_t* Win32SplashScreen::GetWindowClass()
{
   if (!_classRegistered)
   {
      WNDCLASS windowClass{};
      windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
      windowClass.lpszClassName = kWindowClassName;
      windowClass.style = CS_HREDRAW | CS_VREDRAW;
      windowClass.cbClsExtra = 0;
      windowClass.cbWndExtra = 0;
      windowClass.hInstance = GetModuleHandle(nullptr);
      windowClass.hIcon = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName = nullptr;
      windowClass.lpfnWndProc = Win32SplashScreen::WndProc;
      RegisterClass(&windowClass);
      _classRegistered = true;
   }

   return kWindowClassName;
}

void Win32SplashScreen::UnregisterWindowClass()
{
   UnregisterClass(kWindowClassName, nullptr);
   _classRegistered = false;
}


void Win32SplashScreen::Destroy()
{
   if (_hWindow)
   {
      // destroy the window after a short delay to allow the window to be painted
      SetTimer(_hWindow, 1, _minimumHideDelayTime, nullptr);
   }
}

Win32SplashScreen* Win32SplashScreen::GetThisFromHandle(HWND const window) noexcept
{
   return reinterpret_cast<Win32SplashScreen*>(GetWindowLongPtr(window, GWLP_USERDATA));
}

HWND Win32SplashScreen::GetHandle() const
{
   return _hWindow;
}
