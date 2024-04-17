// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// The way dart handles calls to native code, prevents the getting the 
// correct reasons why Win32 APIs fails.
// 
// All Win32 APIs where we do need GetLastError() to deliver the correct failure 
// reasons must be wrapped
//
// Root cause: https://github.com/dart-lang/sdk/issues/38832
// -----------------------------------------------------------------------------
#pragma once

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include "DllExports.h"

// -----------------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

/// <summary>
/// Last Error aware wrapper for the Win32 CreateEventW API
/// </summary>
/// <param name="bManualReset">Same as Win32 API - see API documentation.</param>
/// <param name="bInitialState">Same as Win32 API - see API documentation.</param>
/// <param name="lpName">Same as Win32 API - see API documentation.</param>
/// <param name="lpLastError">DWORD receiving the GetLastError value if the API failed.</param>
/// <returns>Same as Win32 API.</returns>
PLUGIN_API _Ret_maybenull_ HANDLE WINAPI Win32CreateEvent
(
   _In_ BOOL bManualReset,
   _In_ BOOL bInitialState,
   _In_opt_ LPCWSTR lpName,
   _Out_opt_ LPDWORD lpLastError
);

/// <summary>
/// Last Error aware wrapper for the Win32 OpenEventW API
/// </summary>
/// <param name="dwDesiredAccess">Same as Win32 API - see API documentation.</param>
/// <param name="bInheritHandle">Same as Win32 API - see API documentation.</param>
/// <param name="lpName">Same as Win32 API - see API documentation.</param>
/// <param name="lpLastError">DWORD receiving the GetLastError value if the API failed.</param>
/// <returns>Same as Win32 API.</returns>
PLUGIN_API _Ret_maybenull_ HANDLE WINAPI Win32OpenEvent(
   _In_ DWORD dwDesiredAccess,
   _In_ BOOL bInheritHandle,
   _In_ LPCWSTR lpName,
   _Out_opt_ LPDWORD lpLastError
);

/// <summary>
/// Last Error aware wrapper for the Win32 SetEvent API
/// </summary>
/// <param name="hEvent">>Same as Win32 API - see API documentation.</param>
/// <returns>ERROR_SUCCESS if successful; the GetLastError value in case of a failure.</returns>
PLUGIN_API DWORD WINAPI Win32SetEvent
(
   _In_ HANDLE hEvent
);

/// <summary>
/// Last Error aware wrapper for the Win32 ResetEvent API
/// </summary>
/// <param name="hEvent">>Same as Win32 API - see API documentation.</param>
/// <returns>ERROR_SUCCESS if successful; the GetLastError value in case of a failure.</returns>
PLUGIN_API DWORD WINAPI Win32ResetEvent
(
   _In_ HANDLE hEvent
);

#ifdef __cplusplus
}
#endif