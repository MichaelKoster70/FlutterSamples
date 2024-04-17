// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Windows Application Model Helper API declarations
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "DllExport.h"

// ----------------------------------------------------------------------------
// Function Declarations
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
/// <summary>
/// Returns whether the modern app model is supported by the running OS.
/// </summary>
/// <returns>TRUE if running with an identity; else FLSE.</returns>
APPMODELHELPER_API BOOL WINAPI AppModelIsSupported();

/// <summary>
/// Returns whether the application is running with an identity.
/// </summary>
/// <returns>TRUE if running with an identity; else FLSE.</returns>
APPMODELHELPER_API BOOL WINAPI AppModelIsRunningWithIdentity();

#ifdef __cplusplus
}
#endif
