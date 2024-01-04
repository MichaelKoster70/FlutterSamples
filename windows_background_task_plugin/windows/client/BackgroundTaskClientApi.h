// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// -----------------------------------------------------------------------------
// Types and constants
// -----------------------------------------------------------------------------

// GUID for the COM background task class
#define CLSID_ComDartBackgroundTask "88F46315-0A89-4911-9199-6BB496C14533"

/// <summary>
/// Enum holding all possible conditions for a background task next to its trigger.
/// </summary>
typedef enum _ConditionType
{
   /// <summary>Invalid condition type.</summary>
   eConditionTypeInvalid = 0,

   /// <summary>Only runs when the user is present.</summary>
   eConditionTypeUserPresent = 1,

   /// <summary>Only runs when the user is not present.</summary>
   eConditionTypeUserNotPresent = 2,

   /// <summary>Only runs when the Internet is available.</summary>
   eConditionTypeInternetAvailable = 3,

   /// <summary>Only runs when the Internet is not available.</summary>
   eConditionTypeInternetNotAvailable = 4,

   /// <summary>Only runs when the user's session is connected.</summary>
   eConditionTypeSessionConnected = 5,

   /// <summary>Only runs when the user's session is disconnected.</summary>
   eConditionTypeSessionDisconnected = 6,

   /// <summary>Only runs when a free (non-metered) network connection is available.</summary>
   eConditionTypeFreeNetworkAvailable = 7
} ConditionType;

// enum holding completion status of a background task

/// <summary>
/// Enum holding completion status of a background task.
/// </summary>
typedef enum _CompletionStatus
{
   /// <summary>Invalid completion status.</summary>
   eCompletionStatusInvalid = 0,

   /// <summary>The background task completed successfully.</summary>
   eCompletionStatusCompleted = 1,

   /// <summary>The background task was canceled.</summary>
   eCompletionStatusCanceled = 2,

   /// <summary>The background task failed to complete due to an error.</summary>
   eCompletionStatusFailed = 3
} CompletionStatus;

// -----------------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------------

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

/// <summary>
/// Registers a one shot timer triggered background task with optional conditions.
/// </summary>
/// <param name="taskClassId">The COM class identifier of the background task provider.</param>
/// <param name="taskName">The name of the task.</param>
/// <param name="freshnessTime">The freshness time in minutes (minimum 15).</param>
/// <param name="oneShot">TRUE if the time event trigger will be used once; FALSE if it will be used each time freshnessTime elapses.</param>
/// <param name="conditionType">The optional condition type array.</param>
/// <returns>The result code, 0 for success</returns>
BACKGROUND_TASK_CLIENT_API
HRESULT RegisterComBackgroundTaskTimer
(
   _In_ GUID* taskClassId,
   _In_ LPCWSTR taskName,
   _In_ UINT32 freshnessTime,
   _In_ BOOL oneShot,
   _In_ ConditionType conditionType[]
);

/// <summary>
/// Unregisters a background task.
/// </summary>
/// <param name="taskName">The name of the task.</param>
/// <returns>The result code, 0 for success</returns>
BACKGROUND_TASK_CLIENT_API
HRESULT UnregisterBackgroundTask
(
   _In_ LPCWSTR taskName
);

/// <summary>
/// Unregisters all background tasks.
/// </summary>
/// <returns>The result code, 0 for success</returns>
BACKGROUND_TASK_CLIENT_API
HRESULT UnregisterAllBackgroundTasks();

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