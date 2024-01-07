// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "pch.h"
#include <flutter/dart_project.h>
#include "flutter_engine_host.h"
#include "utils.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------
static constexpr char kBackgroundCallbackEntryPoint[] = "backgroundCallback";

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
flutter::DartProject g_project(L"data");

// ----------------------------------------------------------------------------
// App entry point
// ----------------------------------------------------------------------------

// Add a DllMain so that the COM server is initialized when the DLL is loaded
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
   UNREFERENCED_PARAMETER(hModule);
   UNREFERENCED_PARAMETER(lpReserved);

   ConfigureDartProject(g_project, kBackgroundCallbackEntryPoint);

   switch (fdwReason)
   {
   default:
      break;
   }

   return TRUE;
}
