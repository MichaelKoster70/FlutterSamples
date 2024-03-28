// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <windows.h>
#include "win32_window.h"
#include <functional>
#include <memory>
#include <string>

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
// Default maximum time the splash screen should be visible in ms.
constexpr const int DefaultMaximumHideDelayTime = 10000;

// ----------------------------------------------------------------------------
// Class declarations
// ----------------------------------------------------------------------------

/// <summary>
/// A class abstraction for a high DPI-aware Win32 layered Window to host a splash screen.
/// </summary>
class Win32SplashScreen
{
public:
   Win32SplashScreen() = default;
   virtual ~Win32SplashScreen();

   /// <summary>
   /// Creates and shows a win32 window with sized according to the IDB_SPLASH_SCREEN resource PNG file.
   /// </summary>
    /// <param name="owner">The window owning the splash screen.</param>
    /// <param name="minimumHideDelayTime">The minimum time the splash screen should be visible in ms.</param>
    /// <param name="maximumHideDelayTime">The maximum time the splash screen should be visible in ms.</param>
    // <returns>Returns true successful,else false.</returns>
   bool Show(const Win32Window& owner, int minimumHideDelayTime, int maximumHideDelayTime = DefaultMaximumHideDelayTime);

   /// <summary>
   /// Hides the slpash sceen window.
   /// </summary>
   void Hide();

private:
   /// <summary>
   /// Arsm the auto hide timer.
   /// </summary>
   void ArmAutoHideTimer();

   /// <summary>
   /// Release OS resources associated with window.
   /// </summary>
   void Destroy();

   /// <summary>
   /// Member function to process messages sent to the window assoiated with this instance..
   /// </summary>
   /// <param name="hWnd">The handle to the window.</param>
   /// <param name="message">The message.</param>
   /// <param name="wParam">Additional message information.</param>
   /// <param name="lParam">Additional message information.</param>
   /// <returns>Depends on the message</returns>
   LRESULT WndProc (HWND hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept;

   /// <summary>
   /// Loads the splash image from the resource file and sets the window size and position.
   /// </summary>
   /// <param name="windows">The window handle.</param>
   /// <param name="ownerRect">The onwer rect.</param>
   static void LoadSplashImage(HWND windows, RECT ownerRect);

   /// <summary>
   /// Centers the window on the owner window.
   /// </summary>
   /// <param name="ownerRect">The ocation and size of the owner.</param>
   /// <param name="sizeSplash">The Size of the splash screen.</param>
   /// <returns>The location where to place the sphash window.</returns>
   static POINT CenterWindow(const RECT& ownerRect, const SIZE& sizeSplash);

   /// <summary>
   /// // Returns the name of the window class, registering the class if it hasn't previously been registered.
   /// </summary>
   /// <returns>The class name.</returns>
   const wchar_t* GetWindowClass();

   /// <summary>
   /// // Unregisters the window class. Should only be called if there are no instances of the window.
   /// </summary>
   void UnregisterWindowClass();

   /// <summary>
   /// Thecallback function, which you define in your application, that processes messages sent to a window. 
   /// </summary>
   /// <param name="hWnd">A handle to the window.</param>
   /// <param name="message">The message.</param>
   /// <param name="wParam">Additional message information.</param>
   /// <param name="lParam">Additional message information.</param>
   /// <returns> Result of the message processing, and depends on the message sent.</returns>
   static LRESULT CALLBACK StaticWndProc(HWND const hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept;

   /// <summary>
   /// Retrieves a class instance pointer for the supplied window handle.
   /// </summary>
   /// <param name="hWnd">The window handle.</param>
   /// <returns>The instanc pointer.</returns>
   static Win32SplashScreen* GetThisFromHandle(HWND const hWnd) noexcept;

   // window handle for top level window.
   HWND _hWindow = nullptr;

   // minimum time the splash screen should be visible in ms.
   int _minimumHideDelayTime = 0;

   // minimum time the splash screen should be visible in ms.
   int _maximumHideDelayTime = 0;

   bool _classRegistered = false;
};
