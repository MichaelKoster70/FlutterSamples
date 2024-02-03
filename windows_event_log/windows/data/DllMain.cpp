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

/// <summary>
/// An optional entry point into a dynamic-link library-
/// </summary>
/// <param name="hModule">A handle to the DLL module. </param>
/// <param name="fdwReason">The reason code that indicates why the DLL entry-point function is being called. </param>
/// <param name="lpReserved">Unused.</param>
/// <returns>When the system calls the DllMain function with the DLL_PROCESS_ATTACH value, the function returns TRUE if it succeeds or FALSE if initialization fails.</returns>
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
   UNREFERENCED_PARAMETER(hModule);
   UNREFERENCED_PARAMETER(lpReserved);

   switch (fdwReason)
   {
   case DLL_PROCESS_ATTACH:
      break;
   case DLL_THREAD_ATTACH:
      break;
   case DLL_THREAD_DETACH:
      break;
   case DLL_PROCESS_DETACH:
      break;
   }
   return TRUE;
}