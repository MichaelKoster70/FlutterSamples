// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Class declarations
// ----------------------------------------------------------------------------

/// <summary>
/// Utility class to determine the process execution mode.
/// </summary>
class ExecutionMode
{
public:
   /// <summary>
   /// Returns a value whether the process is running with a package identity.
   /// </summary>
   /// <returns>true if the process has package identity.</returns>
   static bool IsRunningWithIdentity();

private:
   static bool IsWindows7OrLower();
   ExecutionMode() = delete;
   ~ExecutionMode() = delete;
   ExecutionMode(const ExecutionMode&) = delete;
};
