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
#include "com_initializer.h"
#include "flutter_engine_host.h"
#include "utils.h"
#include "debug_log.h"

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
//#include "winrt/Windows.ApplicationModel.h"
#include "winrt/Windows.ApplicationModel.AppService.h"


// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
static constexpr char kBackgroundCallbackEntryPoint[] = "backgroundCallback";

// Message sent to the main thread to notify that the channel is initialized.
const UINT kNotifyChannelInitialized = WM_USER + 2;

// Message sent to the main thread to notify that the task is completed.
const UINT kExecuteTaskCompleted = WM_USER + 3;

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
static void HandleNotifyChannelInitialized(FlutterEngineHost& engineHost, const std::wstring& taskName);
static void HandleExecuteTaskCompleted(FlutterEngineHost& engineHost, bool result);

// ----------------------------------------------------------------------------
// App entry point
// ----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(pCmdLine);
   UNREFERENCED_PARAMETER(nCmdShow);

   OutputDebugStringW(pCmdLine);

   // Create a console for the process
   CreateAndAttachConsoleIfNeeded();

   // Initialize COM, so that it is available for use in the flutter engine and plugins.
   auto comInitializer = ComInitializer(COINIT_APARTMENTTHREADED);

   // create and configure
   flutter::DartProject project(L"data");
   ConfigureDartProject(project, kBackgroundCallbackEntryPoint);

   // Set the task name based on the command line arguments
   // UWP LaunchFullTrustProcessForCurrentAppAsync passes the task name as the 3rd  argument
   auto args = GetCommandLineArguments();
   std::wstring taskName;
   if (args.size() >= 3)
   {
      taskName = args[2];
      DebugLog("NATIVE::UWP_RUNNER:: taskName={}", winrt::to_string(taskName));
   }

   // Create and configure the engine host
   FlutterEngineHost engineHost(project);

   engineHost.SetNotifyChannelInitializedHandler([&engineHost, &taskName]() {
      engineHost.PostTaskMessage(kNotifyChannelInitialized, NULL, NULL);
      });

   engineHost.SetTaskMessageHandler([&engineHost, taskName](UINT message, WPARAM wParam, LPARAM lParam)
      {
         switch (message)
         {
         case kNotifyChannelInitialized:
            HandleNotifyChannelInitialized(engineHost,taskName);
            return false;

         case kExecuteTaskCompleted:
            HandleExecuteTaskCompleted(engineHost, wParam);
            return false;

         default:
            return true;
         }
      });

   auto result = engineHost.Run(taskName);

   DebugLog("NATIVE::UWP_RUNNER:: engineHost.Run() returned {}", result);
   return result;
}

/// <summary>
/// Handles the notification that the channel is initialized.
/// </summary>
/// <param name="taskName">The name of the DART task </param>
static void HandleNotifyChannelInitialized(FlutterEngineHost& engineHost, const std::wstring& taskName)
{
   auto taskNameUtf8 = Utf8FromUtf16(taskName);
   DebugLog("NATIVE::HandleNotifyChannelInitialized: taskNameUtf8={}", taskNameUtf8);

   engineHost.GetChannel()->ExecuteTask(taskNameUtf8, [&engineHost](bool result)
      {
         engineHost.PostTaskMessage(kExecuteTaskCompleted, result, NULL);
      });
}

/// <summary>
/// Handles the notification that the task is completed.
/// </summary>
/// <param name="result">The result of the task.</param>
void HandleExecuteTaskCompleted(FlutterEngineHost& engineHost, bool result)
{
   DebugLog("NATIVE::HandleExecuteTaskCompleted");
   engineHost.Shutdown();
}
