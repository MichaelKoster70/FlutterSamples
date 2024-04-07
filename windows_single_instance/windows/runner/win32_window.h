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

#include <functional>
#include <memory>
#include <string>

// ----------------------------------------------------------------------------
// Class declarations
// ----------------------------------------------------------------------------

/// <summary>
/// A class abstraction for a high DPI-aware Win32 Window.
/// Intended to be inherited from by classes that wish to specialize with custom rendering and input handling.
/// </summary>
class Win32Window
{
public:
   struct Point
   {
      unsigned int x;
      unsigned int y;
      Point(unsigned int x, unsigned int y) : x(x), y(y) {}
   };

   struct Size
   {
      unsigned int width;
      unsigned int height;
      Size(unsigned int width, unsigned int height)
         : width(width), height(height) {}
   };

  Win32Window();
  virtual ~Win32Window();

  /// <summary>
  /// Creates a win32 window with |title| that is positioned and sized using |origin| and |size|. 
  /// New windows are created on the default monitor. Window sizes are specified to the OS in physical pixels, 
  /// hence to ensure a consistent size this function will scale the inputted width and height as
  /// as appropriate for the default monitor. The window is invisible until |Show| is called. 
  /// </summary>
   /// <param name="title">The windows title to display.</param>
  /// <param name="origin">The position where to show the window.</param>
  /// <param name="size">the initial size of the windows.</param>
  /// <returns>Returns true if the window was created successfully.</returns>
  bool Create(const std::wstring& title, const Point& origin, const Size& size);

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
  /// Inserts the flutter surface into the window tree.
  /// </summary>
  /// <param name="content">Window hande representing the flutter surface.</param>
  void SetChildContent(HWND hContentWindow);

  /// <summary>
  /// Returns the backing Window handle to enable clients to set icon and other window properties.
  /// </summary>
  /// <returns>The Window handle,nullptr if the window has been destroyed.</returns>
  HWND GetHandle() const;

  /// <summary>
  /// Configures whether closing the window will quit the application.
  /// </summary>
  /// <param name="quitOnClose">true to quit on close, else false</param>
  void SetQuitOnClose(bool quitOnClose);

  /// <summary>
  /// Return a RECT representing the bounds of the current client area.
  /// </summary>
  /// <returns>The RECT holding the client area.</returns>
  RECT GetClientArea();

  /// <summary>
  /// Gets the class name of the window.
  /// </summary>
  /// <returns>The class name.</returns>
  static std::wstring GetWindowClassName();

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

    /// <summary>
    /// Called at the end of Create, allowing subclass window-related setup.
    /// </summary>
    /// <returns>true if successful, else false</returns>
  virtual bool OnCreate();

  /// <summary>
  /// Called when Destroy is called.
  /// </summary>
  virtual void OnDestroy();

 private:
  friend class WindowClassRegistrar;

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
  static Win32Window* GetThisFromHandle(HWND const hWnd) noexcept;

  // Update the window frame's theme to match the system theme.
  static void UpdateTheme(HWND const window);

  bool _quitOnClose = false;

  // window handle for top level window.
  HWND _hWindow = nullptr;

  // window handle for hosted content.
  HWND _hContentWindow = nullptr;

  // The number of Win32Window objects that currently exist.
  static int s_activeWindowCount;
};
