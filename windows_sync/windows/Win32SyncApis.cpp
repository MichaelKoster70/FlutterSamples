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

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include "framework.h"
#include "Win32SyncApis.h"

// -----------------------------------------------------------------------------
// Function implementations
// -----------------------------------------------------------------------------

_Ret_maybenull_ HANDLE WINAPI Win32CreateEvent
(
   _In_ BOOL bManualReset,
   _In_ BOOL bInitialState,
   _In_opt_ LPCWSTR lpName,
   _Out_opt_ LPDWORD lpLastError
)
{
   HANDLE hEvent = CreateEvent(nullptr, bManualReset, bInitialState, lpName);
   if (lpLastError != nullptr)
   {
      *lpLastError = hEvent == nullptr ? GetLastError() : ERROR_SUCCESS;
   }

   return hEvent;
}

_Ret_maybenull_ HANDLE WINAPI Win32OpenEvent
(
   _In_ DWORD dwDesiredAccess,
   _In_ BOOL bInheritHandle,
   _In_ LPCWSTR lpName,
   _Out_opt_ LPDWORD lpLastError
)
{
   HANDLE hEvent = OpenEvent(dwDesiredAccess, bInheritHandle, lpName);
   if (lpLastError != nullptr)
   {
      *lpLastError = hEvent == nullptr ? GetLastError() : ERROR_SUCCESS;
   }

   return hEvent;
}

DWORD WINAPI Win32SetEvent
(
   _In_ HANDLE hEvent
)
{
   BOOL success = SetEvent(hEvent);
   return success ? ERROR_SUCCESS : GetLastError();
}

DWORD WINAPI Win32ResetEvent
(
   _In_ HANDLE hEvent
)
{
   BOOL success = ResetEvent(hEvent);
   return success ? ERROR_SUCCESS : GetLastError();
}