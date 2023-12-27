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
#include <windows.h>
#include <unknwn.h>
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.ApplicationModel.Background.h"
#include "flutter_engine_host.h"
#include "utils.h"
#include "com_initializer.h"
#include "com_dart_background_task.h"
#include "com_register_process_for_type.h"

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
 flutter::DartProject g_project(L"data");

// ----------------------------------------------------------------------------
// App entry point
// ----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(pCmdLine);
   UNREFERENCED_PARAMETER(nCmdShow);

   // Create a console for the process
   CreateAndAttachConsoleIfNeeded();

   // Initialize WinRT COM, so that it is available for use in the flutter engine and plugins.
   auto comInitializer = ComInitializer();

   // Configure the Dart project
   ConfigureDartProject(g_project);

   // Register the COM server for the background task
   ComRegisterProcessForType<ComDartBackgroundTask> registerType;

   registerType.Register();

   //FlutterEngineHost host(project);
   //return host.Run(L"windows_background_task_host", false);

   // run the main message loop
   ::MSG msg;
   while (::GetMessage(&msg, nullptr, 0, 0))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   return static_cast<int>(msg.wParam);
}