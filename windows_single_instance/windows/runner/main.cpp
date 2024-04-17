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

#include "flutter_window.h"
#include "win32_single_instance_safeguard.h"
#include "utils.h"

// ----------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------

int APIENTRY wWinMain(_In_ [[maybe_unused]] HINSTANCE hInstance, _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance, _In_ [[maybe_unused]] PWSTR pCmdLine, _In_ [[maybe_unused]] int nCmdShow)
{
   Win32SingleInstanceSafeguard appInstance(L"Windows_Single_Instance", Win32Window::GetWindowClassName());

   //NOSONAR
   if (appInstance.IsAnotherInstanceRunning(hInstance)) //NOSONAR(cpp:S6004)
   {
      // exit if there is another instance running
      return EXIT_FAILURE;
   }

   CreateAndAttachConsoleIfNeeded();

   // Initialize COM, so that it is available for use in the library and/or plugins.
   (void)::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

   flutter::DartProject project(L"data");

   auto commandLineArguments = GetCommandLineArguments();

   project.set_dart_entrypoint_arguments(std::move(commandLineArguments));

  FlutterWindow window(project);
  Win32Window::Point origin(10, 10);
  Win32Window::Size size(1280, 720);
  if (!window.Create(L"Windows Single Instance", origin, size))
  {
    return EXIT_FAILURE;
  }
  window.SetQuitOnClose(true);

  window.GetHandle();

   ::MSG msg;
   while (::GetMessage(&msg, nullptr, 0, 0))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   ::CoUninitialize();
   return EXIT_SUCCESS;
}
