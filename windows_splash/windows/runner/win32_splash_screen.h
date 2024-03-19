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
  /// Creates a win32 window with |title| that is positioned and sized using |origin| and |size|. 
  /// New windows are created on the default monitor. Window sizes are specified to the OS in physical pixels, 
  /// hence to ensure a consistent size this function will scale the inputted width and height as
  /// as appropriate for the default monitor. The window is invisible until |Show| is called. 
  /// </summary>
   /// <param name="owner">The window owning the splash screen.</param>
  /// <param name="origin">The position where to show the window.</param>
  /// <param name="size">the initial size of the windows.</param>
  /// <returns>Returns true if the window was created successfully.</returns>
  bool Create(const Win32Window& owner);

  /// <summary>
  /// Show the current window.
  /// </summary>
  /// <returns>Returns true if the window was successfully shown.</returns>
  bool Show();

  /// <summary>
  /// Release OS resources associated with window.
  /// </summary>
  void Destroy();

  /// <summary>
  /// Returns the backing Window handle to enable clients to set icon and other window properties.
  /// </summary>
  /// <returns>The Window handle,nullptr if the window has been destroyed.</returns>
  HWND GetHandle() const;

  /// <summary>
  /// Return a RECT representing the bounds of the current client area.
  /// </summary>
  /// <returns>The RECT holding the client area.</returns>
  RECT GetClientArea();

 protected:
    /// <summary>
    /// Processes and route salient window messages for mouse handling, size change and DPI.
    /// Delegates handling of these to member overloads that inheriting classes can handle.
    /// </summary>
    /// <param name="hWnd">The handle to the window.</param>
    /// <param name="message">The message.</param>
    /// <param name="wParam">Additional message information.</param>
    /// <param name="lParam">Additional message information.</param>
    /// <returns>Depends on the message</returns>
    virtual LRESULT MessageHandler(HWND hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept;

 private:

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
  /// A callback function, which you define in your application, that processes messages sent to a window. 
  /// </summary>
  /// <param name="hWnd">A handle to the window</param>
  /// <param name="message">The message</param>
  /// <param name="wParam">Additional message information</param>
  /// <param name="lParam">Additional message information</param>
  /// <returns> Result of the message processing, and depends on the message sent.</returns>
  /// <remarks>Handles the WM_NCCREATE message which is passed when the non-client area is being created and enables 
  /// automatic non-client DPI scaling so that the non-client area automatically responds to changes in DPI. 
  /// All other messages are handled by MessageHandler.
  /// </remarks>
  static LRESULT CALLBACK WndProc(HWND const hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept;

  /// <summary>
  /// Retrieves a class instance pointer for |hWnd|.
  /// </summary>
  /// <param name="hWnd">The window handle.</param>
  /// <returns>The instanc pointer.</returns>
  static Win32SplashScreen* GetThisFromHandle(HWND const hWnd) noexcept;

  // Update the window frame's theme to match the system theme.
  static void UpdateTheme(HWND const window);

  // window handle for top level window.
  HWND _hWindow = nullptr;

  bool _classRegistered = false;
};
