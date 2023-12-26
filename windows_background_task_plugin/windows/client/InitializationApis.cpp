// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Holds the implementation of all inialization and uninitialization functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "DllExports.h"
#include <windows.h>
#include <string>
#include "BackgroundTaskClientApi.h"
#include "winrt/Windows.Foundation.h"

// ----------------------------------------------------------------------------
// Externals
// ----------------------------------------------------------------------------
extern std::wstring g_applicationId;

// ----------------------------------------------------------------------------
// API Implementation
// ----------------------------------------------------------------------------

HRESULT Initialize
(
   _In_ LPCWSTR applicationId
)
{
   try
   {
      /// Initialize the Windows Runtime, may throw an exception
      winrt::init_apartment();

      // Save the application ID
      g_applicationId= applicationId;
   }
   catch (...)
   {
      // return the error code
      return winrt::to_hresult();
   }

   return S_OK;
}

void Uninitialize()
{
   winrt::uninit_apartment();
}