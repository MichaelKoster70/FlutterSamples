// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// -----------------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

/// <summary>
/// Initializes the background task client.
/// </summary>
/// <param name="applicationId">The application identifier.</param>
/// <remarks>
/// This function must be called before any other function in this library.
/// This function will initialite the Windows Runtime COM infrastructure in multithreaded apartment mode.
/// <returns>The result code, 0 for success</returns>
BACKGROUND_TASK_CLIENT_API 
HRESULT Initialize
(
   _In_ LPCWSTR applicationId
);

// register a one time time triggere task with no conditions


/// <summary>
/// Registers a one shot timer triggered background task with no conditions.
/// </summary>
/// <param name="taskName">The name of the task.</param>
/// <param name="freshnessTime">The freshness time in minutes (minimum 15).</param>
/// <param name="oneShot">TRUE if the time event trigger will be used once; FALSE if it will be used each time freshnessTime elapses.</param>
/// <returns>The result code, 0 for success</returns>
BACKGROUND_TASK_CLIENT_API
HRESULT RegisterBackgroundTaskTimerNoConditions
(
   _In_ LPCWSTR taskName,
   _In_ UINT32 freshnessTime,
   _In_ BOOL oneShot
);

/// <summary>
/// Uninitializes the background task client.
/// </summary>
/// <remarks>
/// This function must be called after all other functions in this library.
/// This function will uninitialite the Windows Runtime COM infrastructure.
BACKGROUND_TASK_CLIENT_API
void Uninitialize();

#ifdef __cplusplus
}
#endif