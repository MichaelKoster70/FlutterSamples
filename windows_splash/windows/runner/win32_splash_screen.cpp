// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "win32_splash_screen.h"
#include <dwmapi.h>
#include "resource.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
/// Window attribute that enables dark mode window decorations.
///
/// Redefined in case the developer's machine has a Windows SDK older than
/// version 10.0.22000.0.
/// See: https://docs.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

constexpr const wchar_t kWindowClassName[] = L"SplashWindow";

/// Registry key for app theme preference.
///
/// A value of 0 indicates apps should use dark mode. A non-zero or missing
/// value indicates apps should use light mode.
constexpr const wchar_t kGetPreferredBrightnessRegKey[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
constexpr const wchar_t kGetPreferredBrightnessRegValue[] = L"AppsUseLightTheme";

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------

Win32SplashScreen::~Win32SplashScreen()
{
   Destroy();
}

bool Win32SplashScreen::Create(const Win32Window& owner)
{
   const wchar_t* wndClass = GetWindowClass();

   auto window = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, wndClass, L"", WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandle(nullptr), this);

   if (!window)
   {
      return false;
   }

   UpdateTheme(window);

   return true;
}

bool Win32SplashScreen::Show()
{
   return ShowWindow(_hWindow, SW_SHOWNORMAL);
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
   case WM_DESTROY:
      _hWindow = nullptr;
      Destroy();

      return 0;

   case WM_DPICHANGED:
   {
      auto newRectSize = reinterpret_cast<RECT*>(lParam);
      LONG newWidth = newRectSize->right - newRectSize->left;
      LONG newHeight = newRectSize->bottom - newRectSize->top;

      SetWindowPos(hWnd, nullptr, newRectSize->left, newRectSize->top, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);

      return 0;
   }


   case WM_DWMCOLORIZATIONCOLORCHANGED:
      UpdateTheme(hWnd);
      return 0;
   }

   return DefWindowProc(_hWindow, message, wParam, lParam);
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
      DestroyWindow(_hWindow);
      _hWindow = nullptr;
   }

   UnregisterWindowClass();
}

Win32SplashScreen* Win32SplashScreen::GetThisFromHandle(HWND const window) noexcept
{
   return reinterpret_cast<Win32SplashScreen*>(GetWindowLongPtr(window, GWLP_USERDATA));
}

RECT Win32SplashScreen::GetClientArea()
{
   RECT frame;
   GetClientRect(_hWindow, &frame);
   return frame;
}

HWND Win32SplashScreen::GetHandle() const
{
   return _hWindow;
}

void Win32SplashScreen::UpdateTheme(HWND const hWindow)
{
   DWORD lightMode = 0;
   DWORD lightModeSize = sizeof(lightMode);
   LSTATUS result = RegGetValue(HKEY_CURRENT_USER, kGetPreferredBrightnessRegKey,
      kGetPreferredBrightnessRegValue, RRF_RT_REG_DWORD, nullptr, &lightMode, &lightModeSize);

   if (result == ERROR_SUCCESS)
   {
      BOOL enableDarkMode = lightMode == 0;
      DwmSetWindowAttribute(hWindow, DWMWA_USE_IMMERSIVE_DARK_MODE, &enableDarkMode, sizeof(enableDarkMode));
   }
}
