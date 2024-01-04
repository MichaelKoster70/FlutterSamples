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

   // Initialize COM, so that it is available for use in the flutter engine and plugins.
   auto comInitializer = ComInitializer(COINIT_MULTITHREADED);

   flutter::DartProject project(L"data");

   ConfigureDartProject(project);

   FlutterEngineHost host(project);
   return host.Run(L"windows_console");
}
