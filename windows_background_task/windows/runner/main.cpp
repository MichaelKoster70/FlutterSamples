// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <windows.h>
#include <iostream>

#include "flutter_window.h"
#include "utils.h"

// ----------------------------------------------------------------------------
// App entry point
// ----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ wchar_t* pCmdLine, _In_ int nCmdShow)
{
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(pCmdLine);
   UNREFERENCED_PARAMETER(nCmdShow);

   CreateAndAttachConsoleIfNeeded();

   // Initialize COM, so that it is available for use in the library and plugins.
   (void)::CoInitializeEx(nullptr, COINIT_MULTITHREADED);

   flutter::DartProject project(L"data");

   ConfigureDartProject(project);

   FlutterWindow window(project);
   Win32Window::Point origin(10, 10);
   Win32Window::Size size(1280, 720);
   if (!window.Create(L"windows_background_task", origin, size))
   {
      return EXIT_FAILURE;
   }
   window.SetQuitOnClose(true);

   ::MSG msg;
   while (::GetMessage(&msg, nullptr, 0, 0))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   ::CoUninitialize();
   return EXIT_SUCCESS;
}
