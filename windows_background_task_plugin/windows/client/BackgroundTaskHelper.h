// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Namespace usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::ApplicationModel::Background;

// ----------------------------------------------------------------------------
// Declaration
// ----------------------------------------------------------------------------

/// <summary>
/// Utility class to register a background tasks
/// </summary>
class BackgroundTaskHelper
{
public:
   BackgroundTaskHelper() = default;
   ~BackgroundTaskHelper() = default;

   /// <summary>
   /// Registers a COM based background task returning the background task registration.
   /// </summary>
   /// <param name="taskName">The task name.</param>
   /// <param name="classId">The COM class ID imnplementing the task.</param>
   /// <param name="trigger">The task trigger to use.</param>
   /// <param name="conditions">The optional task conditions to use.</param>
   /// <returns>The background task registration instance.</returns>
   static IBackgroundTaskRegistration RegisterAndGetRegistration(hstring taskName, guid classId, IBackgroundTrigger trigger, IVector<IBackgroundCondition> conditions);

   /// <summary>
   /// Registers a COM based background task.
   /// </summary>
   /// <param name="taskName">The task name.</param>
   /// <param name="classId">The COM class ID imnplementing the task.</param>
   /// <param name="trigger">The task trigger to use.</param>
   /// <param name="conditions">The optional task conditions to use.</param>
   static void Register(hstring taskName, guid classId, IBackgroundTrigger trigger, IVector<IBackgroundCondition> conditions);

   /// <summary>
   /// Unregisters a background task.
   /// </summary>
   /// <param name="taskName">The task name.</param>
   static void Unregister(hstring taskName);

private:
   /// <summary>
   /// Returns a value indicating whether the task is registered.
   /// </summary>
   /// <param name="taskName">The task name.</param>
   /// <returns>true if registered, else false.</returns>
   static bool IsTaskRegistered(hstring taskName);

   /// <summary>
   /// Gets the task registration.
   /// </summary>
   /// <param name="taskName">The task name.</param>
   /// <returns>The background task registration instance.</returns>
   static IBackgroundTaskRegistration GetTaskRegistration(hstring taskName);
};