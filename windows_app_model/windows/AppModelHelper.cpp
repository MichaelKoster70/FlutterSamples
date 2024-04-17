// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Windows Application Model Helper API definitions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "framework.h"
#include "AppModelHelper.h"
#include <VersionHelpers.h>
#include <appmodel.h>
#include <string>

// ----------------------------------------------------------------------------
// Function Definitions
// ----------------------------------------------------------------------------

BOOL WINAPI AppModelIsSupported()
{
   return IsWindows7OrGreater();
}

BOOL WINAPI AppModelIsRunningWithIdentity()
{
   if (IsWindows7OrGreater())
   {
      auto buffer = std::wstring(1024, L'\0');
      UINT32 length = 0;
      int result = GetCurrentPackageFullName(&length, buffer.data());

      return result != APPMODEL_ERROR_NO_PACKAGE;
   }

   return FALSE;
}
