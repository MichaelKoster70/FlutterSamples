// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "flutter_engine_host.h"

// ----------------------------------------------------------------------------
// Usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::ApplicationModel::Background;

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------

// GUID for the background task class
#define CLSID_ComDartBackgroundTask "88F46315-0A89-4911-9199-6BB496C14533"

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------

/// <summary>
/// Class providing the COM interface for the background task.
/// </summary>
struct __declspec(uuid(CLSID_ComDartBackgroundTask))
   ComDartBackgroundTask : implements<ComDartBackgroundTask, IBackgroundTask>
{
   ComDartBackgroundTask();

   /// <summary>
   ///  Performs the work of a background task. The system calls this method when the associated background task has been triggered.
   /// 
   /// </summary>
   /// <param name="taskInstance">An interface to an instance of the background task. The system creates this instance when the task has been triggered to run.</param>
   void __stdcall Run(_In_ IBackgroundTaskInstance taskInstance);

   /// <summary>
   /// Handles the cancellation of a background task.
   /// </summary>
   /// <param name="sender">The background task instance that was cancelled.</param>
   /// <param name="">The reason the background task instance was cancelled.</param>
   void __stdcall OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason);

private:
   FlutterEngineHost engineHost;
   BackgroundTaskDeferral taskDeferral = nullptr;
   volatile bool isCanceled = false;
};
