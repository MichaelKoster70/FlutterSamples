// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "win32_window.h"

#include <dwmapi.h>
#include <flutter_windows.h>
#include "resource.h"

// ----------------------------------------------------------------------------
// Defines and macros
// ----------------------------------------------------------------------------

/// Window attribute that enables dark mode window decorations.
///
/// Redefined in case the developer's machine has a Windows SDK older than
/// version 10.0.22000.0.
/// See: https://docs.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
constexpr const wchar_t kWindowClassName[] = L"FLUTTER_RUNNER_WIN32_WINDOW";

/// Registry key for app theme preference.
///
/// A value of 0 indicates apps should use dark mode. A non-zero or missing value indicates apps should use light mode.
constexpr const wchar_t kGetPreferredBrightnessRegKey[] =  L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
constexpr const wchar_t kGetPreferredBrightnessRegValue[] = L"AppsUseLightTheme";

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
// The number of Win32Window objects that currently exist.
static int g_activeWindowCount = 0;

// ----------------------------------------------------------------------------
// Helper functions
// ----------------------------------------------------------------------------

/// <summary>
///  Scale helper to convert logical scaler values to physical using passed in scale factor
/// </summary>
/// <param name="source">Source value to scale</param>
/// <param name="scaleFactor">Scale factor</param>
/// <returns>Scaled value</returns>
int Scale(int source, double scaleFactor)
{
  return static_cast<int>(source * scaleFactor);
}

/// <summary>
/// Enable DPI scaling for the non-client area of the window.
/// This API is only needed for PerMonitor V1 awareness mode.
/// </summary>
/// <param name="hWnd">Windows handle to enable for.</param>
void EnableFullDpiSupportIfAvailable(HWND hWnd)
{
   using EnableNonClientDpiScaling = BOOL __stdcall(HWND hwnd);

   HMODULE user32Module = LoadLibraryA("User32.dll");
   if (!user32Module)
   {
      return;
   }
   auto enableNonClientDpiScaling = reinterpret_cast<EnableNonClientDpiScaling*>(GetProcAddress(user32Module, "EnableNonClientDpiScaling"));
   if (enableNonClientDpiScaling != nullptr)
   {
      enableNonClientDpiScaling(hWnd);
   }
   FreeLibrary(user32Module);
}

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------

/// <summary>
/// Manages the Win32Window's window class registration.
/// </summary>
class WindowClassRegistrar
{
public:
   ~WindowClassRegistrar() = default;

   // Returns the singleton registrar instance.
   static WindowClassRegistrar* GetInstance()
   {
      if (!_instance)
      {
         _instance = new WindowClassRegistrar();
      }
      return _instance;
   }

   /// <summary>
   /// Returns the name of the window class, registering the class if it hasn't previously been registered.
   /// </summary>
   /// <returns>The windows class name</returns>
   const wchar_t* GetWindowClass()
   {
      if (!_classRegistered)
      {
         WNDCLASS window_class{};
         window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
         window_class.lpszClassName = kWindowClassName;
         window_class.style = CS_HREDRAW | CS_VREDRAW;
         window_class.cbClsExtra = 0;
         window_class.cbWndExtra = 0;
         window_class.hInstance = GetModuleHandle(nullptr);
         window_class.hIcon = LoadIcon(window_class.hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
         window_class.hbrBackground = 0;
         window_class.lpszMenuName = nullptr;
         window_class.lpfnWndProc = Win32Window::WndProc;
         RegisterClass(&window_class);
         _classRegistered = true;
      }

      return kWindowClassName;
   }

   /// <summary>
   /// Unregisters the window class. Should only be called if there are no instances of the window.
   /// </summary>
   void UnregisterWindowClass()
   {
      UnregisterClass(kWindowClassName, nullptr);
      _classRegistered = false;
   };

private:
   WindowClassRegistrar() = default;

   static WindowClassRegistrar* _instance;
   bool _classRegistered = false;
};

WindowClassRegistrar* WindowClassRegistrar::_instance = nullptr;


Win32Window::Win32Window()
{
   ++g_activeWindowCount;
}

Win32Window::~Win32Window()
{
   --g_activeWindowCount;
   Destroy();
}

bool Win32Window::Create(const std::wstring& title, const Point& origin, const Size& size)
{
   Destroy();

   const wchar_t* windowClass = WindowClassRegistrar::GetInstance()->GetWindowClass();

   const POINT target_point = { static_cast<LONG>(origin.x), static_cast<LONG>(origin.y) };
   HMONITOR monitor = MonitorFromPoint(target_point, MONITOR_DEFAULTTONEAREST);
   UINT dpi = FlutterDesktopGetDpiForMonitor(monitor);
   double scale_factor = dpi / 96.0;

   HWND hWindow = CreateWindow(
      windowClass, title.c_str(), WS_OVERLAPPEDWINDOW,
      Scale(origin.x, scale_factor), Scale(origin.y, scale_factor),
      Scale(size.width, scale_factor), Scale(size.height, scale_factor),
      nullptr, nullptr, GetModuleHandle(nullptr), this);

   if (!hWindow)
   {
      return false;
   }

   UpdateTheme(hWindow);

   return OnCreate();
}

bool Win32Window::Show()
{
   return ShowWindow(_hWindow, SW_SHOWNORMAL);
}

// static
LRESULT CALLBACK Win32Window::WndProc(HWND const hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept
{
   if (message == WM_NCCREATE)
   {
      // Store the Win32Window instance pointer in the window's user data for future use.
      auto wndStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
      SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wndStruct->lpCreateParams));

      auto that = static_cast<Win32Window*>(wndStruct->lpCreateParams);
      EnableFullDpiSupportIfAvailable(hWnd);
      that->_hWindow = hWnd;
   }
   else if (Win32Window* that = GetThisFromHandle(hWnd))
   {
      return that->MessageHandler(hWnd, message, wParam, lParam);
   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Win32Window::MessageHandler(HWND hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
{
   switch (message)
   {
   case WM_DESTROY:
      _hWindow = nullptr;
      Destroy();
      if (_quitOnClose)
      {
         PostQuitMessage(0);
      }
      return 0;

   case WM_DPICHANGED:
   {
      auto newRectSize = reinterpret_cast<RECT*>(lparam);
      LONG newWidth = newRectSize->right - newRectSize->left;
      LONG newHeight = newRectSize->bottom - newRectSize->top;

      SetWindowPos(hwnd, nullptr, newRectSize->left, newRectSize->top, newWidth,
         newHeight, SWP_NOZORDER | SWP_NOACTIVATE);

      return 0;
   }

   case WM_SIZE:
   {
      RECT rect = GetClientArea();
      if (_hContentWindow != nullptr)
      {
         // Size and position the child window.
         MoveWindow(_hContentWindow, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
      }
      return 0;
   }

   case WM_ACTIVATE:
      if (_hContentWindow != nullptr)
      {
         SetFocus(_hContentWindow);
      }
      return 0;

   case WM_DWMCOLORIZATIONCOLORCHANGED:
      UpdateTheme(hwnd);
      return 0;
   }

   return DefWindowProc(_hWindow, message, wparam, lparam);
}

void Win32Window::Destroy()
{
   OnDestroy();

   if (_hWindow)
   {
      DestroyWindow(_hWindow);
      _hWindow = nullptr;
   }

   if (g_activeWindowCount == 0)
   {
      WindowClassRegistrar::GetInstance()->UnregisterWindowClass();
   }
}

Win32Window* Win32Window::GetThisFromHandle(HWND const window) noexcept
{
   return reinterpret_cast<Win32Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
}

void Win32Window::SetChildContent(HWND hContentWindow)
{
   _hContentWindow = hContentWindow;
   SetParent(_hContentWindow, _hWindow);
   RECT frame = GetClientArea();

   MoveWindow(hContentWindow, frame.left, frame.top, frame.right - frame.left, frame.bottom - frame.top, true);

   SetFocus(_hContentWindow);
}

RECT Win32Window::GetClientArea()
{
   RECT frame;
   GetClientRect(_hWindow, &frame);
   return frame;
}

HWND Win32Window::GetHandle()
{
   return _hWindow;
}

void Win32Window::SetQuitOnClose(bool quitOnClose)
{
   _quitOnClose = quitOnClose;
}

void Win32Window::UpdateTheme(HWND const window)
{
   DWORD lightMode;
   DWORD lightModeSize = sizeof(lightMode);
   LSTATUS result = RegGetValue(HKEY_CURRENT_USER, kGetPreferredBrightnessRegKey,
      kGetPreferredBrightnessRegValue, RRF_RT_REG_DWORD, nullptr, &lightMode, &lightModeSize);

   if (result == ERROR_SUCCESS)
   {
      BOOL enableDarkMode = lightMode == 0;
      DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE, &enableDarkMode, sizeof(enableDarkMode));
   }
}
