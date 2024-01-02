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
// Constants
// ----------------------------------------------------------------------------
static constexpr char kBackgroundCallbackEntryPoint[] = "backgroundCallback";

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
flutter::DartProject g_project(L"data");

/// <summary>Manual Reset Event that gets signaled when the process should exit.</summary>
handle g_processExitEvent{ CreateEvent(nullptr, TRUE, FALSE, nullptr) };

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

   // Configure the Dart project with a custom entrypoint for the background task
   ConfigureDartProject(g_project, kBackgroundCallbackEntryPoint);

   // Register the COM server for the background task
   ComRegisterProcessForType<ComDartBackgroundTask> registerType;

   registerType.Register();

   //FlutterEngineHost::SetMainThread(::GetCurrentThreadId());

   // run the main message loop
   //::MSG msg;
   //while (::GetMessage(&msg, nullptr, 0, 0))
   //{
   //   if (FlutterEngineHost::ThreadMessageHandler(msg.message, msg.wParam, msg.lParam) != 0)
   //   {
   //      ::TranslateMessage(&msg);
   //      ::DispatchMessage(&msg);
   //   }
   //}

   //return static_cast<int>(msg.wParam);

   return 0;
}