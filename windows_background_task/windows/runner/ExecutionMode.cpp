// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "ExecutionMode.h"
#include <Windows.h>
#include <appmodel.h>
#include <string>
#include <VersionHelpers.h>

// ----------------------------------------------------------------------------
// Class implementation
// ----------------------------------------------------------------------------

bool ExecutionMode::IsRunningWithIdentity()
{
   if (IsWindows7OrGreater())
   {
      auto buffer = std::wstring(1024, L'\0');
      UINT32 length = 0;
      int result = GetCurrentPackageFullName(&length, buffer.data());

      return result != APPMODEL_ERROR_NO_PACKAGE;
   }
   return false;
}
