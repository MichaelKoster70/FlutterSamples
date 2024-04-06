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
constexpr const wchar_t g_WindowClassName[] = L"SplashWindow";
constexpr const int g_timerId = 1;

// ----------------------------------------------------------------------------
// Linker directives
// ----------------------------------------------------------------------------
#pragma comment(lib, "Shcore.lib")

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------

Win32SplashScreen::~Win32SplashScreen()
{
   Destroy();
}

bool Win32SplashScreen::Show(const Win32Window& owner, int minimumHideDelayTime, int maximumHideDelayTime)
{
   _minimumHideDelayTime = minimumHideDelayTime;
   _maximumHideDelayTime = maximumHideDelayTime;
   const wchar_t* wndClass = GetWindowClass();

   // Create the layered window that will display the splash screen
   auto hWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, wndClass, L"", WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandle(nullptr), this);

   if (!hWindow)
   {
      return false;
   }

   // Show the window, on top of the owner window
   RECT ownerRect{};
   GetWindowRect(owner.GetHandle(), &ownerRect);
   LoadSplashImage(hWindow, ownerRect);
   ArmAutoHideTimer();

   return true;
}

void Win32SplashScreen::Hide()
{
   if (_hWindow)
   {
      // destroy the window after a short delay to allow the window to be painted
      SetTimer(_hWindow, g_timerId, _minimumHideDelayTime, nullptr);
   }
}

LRESULT CALLBACK Win32SplashScreen::StaticWndProc(HWND const hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept
{
   if (message == WM_NCCREATE)
   {
      // store the 'this' pointer in the window user data
      auto wndStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
      SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndStruct->lpCreateParams));

      auto instance = static_cast<Win32SplashScreen*>(wndStruct->lpCreateParams);
      EnableNonClientDpiScaling(hWnd);
      instance->_hWindow = hWnd;
   }
   else if (auto instance = GetThisFromHandle(hWnd))
   {
      return instance->WndProc(hWnd, message, wParam, lParam);
   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Win32SplashScreen::WndProc(HWND hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept
{
   switch (message)
   {
   default:
      break;

   case WM_TIMER:
      Destroy();
      return 0;

   case WM_DESTROY:
      _hWindow = nullptr;
      Destroy();
      return 0;

   case WM_DPICHANGED:
      auto newRectSize = reinterpret_cast<RECT*>(lParam);
      LONG newWidth = newRectSize->right - newRectSize->left;
      LONG newHeight = newRectSize->bottom - newRectSize->top;

      SetWindowPos(hWnd, nullptr, newRectSize->left, newRectSize->top, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);

      return 0;
   }

   return DefWindowProc(_hWindow, message, wParam, lParam);
}

void Win32SplashScreen::LoadSplashImage(HWND windows, RECT ownerRect)
{
   ImageLoader loader;

   double scaleFactor = 1.0;
   LPCTSTR resource = GetResourceWithScale(ownerRect, scaleFactor);

   HBITMAP hBitmap = loader.Load(resource);
   if (hBitmap)
   {
      POINT ptZero { 0 };
      BITMAP bitmap{};
      GetObject(hBitmap, sizeof(BITMAP), &bitmap);
      SIZE sizeSplash{ static_cast<LONG>(bitmap.bmWidth * scaleFactor), static_cast<LONG>(bitmap.bmHeight * scaleFactor) };

      auto origin = CenterWindow(ownerRect, sizeSplash);

      HDC hdcScreen = GetDC(nullptr);
      HDC hdcMem = CreateCompatibleDC(hdcScreen);
      auto hbmOld = SelectObject(hdcMem, hBitmap);

      // Members intialized as BlendOp = AC_SRC_OVER, SourceConstantAlpha = 255, AlphaFormat = AC_SRC_ALPHA, BlendFlags = 0
      BLENDFUNCTION blendFunction{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
      UpdateLayeredWindow(windows, hdcScreen, &origin, &sizeSplash, hdcMem, &ptZero, RGB(0, 0, 0), &blendFunction, ULW_ALPHA);

      SelectObject(hdcMem, hbmOld);
      DeleteDC(hdcMem);
      ReleaseDC(nullptr, hdcScreen);
      DeleteObject(hBitmap);
   }
}

LPCTSTR Win32SplashScreen::GetResourceWithScale(const RECT& ownerRect, double& scaleFactor)
{
   const POINT targetPoint = { ownerRect.left, ownerRect.top };
   HMONITOR monitor = MonitorFromPoint(targetPoint, MONITOR_DEFAULTTONEAREST);

   if (DEVICE_SCALE_FACTOR deviceScaleFactor{}; SUCCEEDED(GetScaleFactorForMonitor(monitor, &deviceScaleFactor)))
   {
      switch (deviceScaleFactor)
      {
      default: // for unknown scale factors, use 100%
         scaleFactor = 1.0;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_100);

      case DEVICE_SCALE_FACTOR::SCALE_120_PERCENT:
         scaleFactor = 0.6;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_125_PERCENT:
         scaleFactor = 0.625;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_140_PERCENT:
         scaleFactor = 0.7;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_150_PERCENT:
         scaleFactor = 0.75;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_160_PERCENT:
         scaleFactor = 0.8;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_175_PERCENT:
         scaleFactor = 0.875;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_180_PERCENT:
         scaleFactor = 0.9;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_200_PERCENT:
         scaleFactor = 1.0;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_200);

      case DEVICE_SCALE_FACTOR::SCALE_225_PERCENT:
         scaleFactor = 0.5625;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);

      case DEVICE_SCALE_FACTOR::SCALE_250_PERCENT:
         scaleFactor = 0.625;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);

      case DEVICE_SCALE_FACTOR::SCALE_300_PERCENT:
         scaleFactor = 0.75;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);

      case DEVICE_SCALE_FACTOR::SCALE_350_PERCENT:
         scaleFactor = 0.875;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);

      case DEVICE_SCALE_FACTOR::SCALE_400_PERCENT:
         scaleFactor = 1.0;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);

      case DEVICE_SCALE_FACTOR::SCALE_450_PERCENT:
         scaleFactor = 1.125;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);

      case DEVICE_SCALE_FACTOR::SCALE_500_PERCENT:
         scaleFactor = 1.25;
         return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_400);
      }
   }
   else
   {
      scaleFactor = 1.0;
      return MAKEINTRESOURCE(IDB_SPLASH_SCREEN_100);
   }
}

POINT Win32SplashScreen::CenterWindow(const RECT& ownerRect, const SIZE& sizeSplash)
{
   auto ownerWidth = ownerRect.right - ownerRect.left;
   auto ownerHeight = ownerRect.bottom - ownerRect.top;

   return POINT { ownerRect.left + (ownerWidth - sizeSplash.cx) / 2, ownerRect.top + (ownerHeight - sizeSplash.cy) / 2 };
}

   // Returns the name of the window class, registering the class if it hasn't previously been registered.
const wchar_t* Win32SplashScreen::GetWindowClass()
{
   if (!_classRegistered)
   {
      WNDCLASS windowClass{};
      windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
      windowClass.lpszClassName = g_WindowClassName;
      windowClass.style = CS_HREDRAW | CS_VREDRAW;
      windowClass.cbClsExtra = 0;
      windowClass.cbWndExtra = 0;
      windowClass.hInstance = GetModuleHandle(nullptr);
      windowClass.hIcon = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
      windowClass.hbrBackground = nullptr;
      windowClass.lpszMenuName = nullptr;
      windowClass.lpfnWndProc = Win32SplashScreen::StaticWndProc;
      RegisterClass(&windowClass);
      _classRegistered = true;
   }

   return g_WindowClassName;
}

void Win32SplashScreen::UnregisterWindowClass()
{
   if (_classRegistered)
   {
      UnregisterClass(g_WindowClassName, nullptr);
      _classRegistered = false;
   }
}

void Win32SplashScreen::ArmAutoHideTimer()
{
   if (_hWindow)
   {
      SetTimer(_hWindow, g_timerId, _maximumHideDelayTime, nullptr);
   }
}

void Win32SplashScreen::Destroy()
{
   if (_hWindow)
   {
      KillTimer(_hWindow, 1);
      DestroyWindow(_hWindow);
   }

   UnregisterWindowClass();
}

Win32SplashScreen* Win32SplashScreen::GetThisFromHandle(HWND const window) noexcept
{
   return reinterpret_cast<Win32SplashScreen*>(GetWindowLongPtr(window, GWLP_USERDATA));
}

