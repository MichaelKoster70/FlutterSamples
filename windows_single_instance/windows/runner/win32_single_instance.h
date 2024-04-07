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
#include <string>

// ----------------------------------------------------------------------------
// Class declarations
// ----------------------------------------------------------------------------

/// <summary>
/// A class providing a mechanism to ensure only a single instance of an application is running.
/// </summary>
class Win32SingleInstanceLock
{
public:

   /// <summary>
   /// Creates a new instance of the Win32SingleInstanceLock class.
   /// If the windowClasName is provided, the existing instance will be activated.
   /// </summary>
   /// <param name="defaultMutexName">The default mutex name.</param>
   /// <param name="windowClasName">The class name.</param>
   Win32SingleInstanceLock(const std::wstring& defaultMutexName, const std::wstring& windowClasName = L"");

   ~Win32SingleInstanceLock();

   /// <summary>
   /// Checks if another instance of the application is running.
   /// </summary>
   /// <param name="hInstance">The current instance handle.</param>
   /// <returns>Returns true if another instance is running.</returns>
   bool IsAnotherInstanceRunning(HINSTANCE hInstance);

private:
   /// <summary>
   /// Gets a mutex name based on the module name.
   /// </summary>
   /// <param name="hInstance">The module instance.</param>
   /// <returns>The mutex name.</returns>
   std::wstring GetMutexNameBasedOnModuleName(HINSTANCE hInstance);

   /// <summary>
   /// Tries to activate the existing instance.
   /// </summary>
   void ActivateExistingInstance();

   /// <summary>
   /// Gets the application exe module name from the supplied module handle.
   /// </summary>
   /// <param name="hInstance">The module instance.</param>
   /// <returns>The absolute path holding the application exe.</returns>
   std::wstring GetModuleNameFromInstance(HINSTANCE hInstance);

   /// <summary>
   /// Gets the application exe module name from the supplied window handle
   /// </summary>
   /// <param name="hWnd">The window handle to examine.</param>
   /// <returns>The absolute path holding the application exe.</returns>
   std::wstring GetModuleNameFromWindow(HWND hWnd);


   Win32SingleInstanceLock(const Win32SingleInstanceLock&) = delete;
   Win32SingleInstanceLock(Win32SingleInstanceLock&&) = delete;
   Win32SingleInstanceLock& operator=(const Win32SingleInstanceLock&) = delete;
   Win32SingleInstanceLock& operator=(Win32SingleInstanceLock&&) = delete;

   // Handle to the mutex.
   HANDLE hMutex = nullptr;

   // The default mutex name.
   std::wstring defaultMutexName;

   // window class name to activate
   std::wstring windowClasName;
};