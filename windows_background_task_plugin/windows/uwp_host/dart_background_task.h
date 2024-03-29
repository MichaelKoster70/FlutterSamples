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
#include "DartBackgroundTask.g.h"

// ----------------------------------------------------------------------------
// Usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::ApplicationModel::Background;

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------

namespace winrt::BackgroundTaskHost::implementation
{
   struct DartBackgroundTask : DartBackgroundTaskT<DartBackgroundTask>
   {
      DartBackgroundTask();

      /// <summary>
      ///  Performs the work of a background task. The system calls this method when the associated background task has been triggered.
      /// </summary>
      /// <param name="taskInstance">An interface to an instance of the background task. The system creates this instance when the task has been triggered to run.</param>
      void Run(IBackgroundTaskInstance const& taskInstance);

      /// <summary>
      /// Handles the cancellation of a background task.
      /// </summary>
      /// <param name="sender">The background task instance that was cancelled.</param>
      /// <param name="reason">The reason the background task instance was cancelled.</param>
      void __stdcall OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason);

   private:
      /// <summary>
      /// Debug helper to show a toast message
      /// </summary>
      /// <param name="message">The message to show</param>
      void DebugShowToast(hstring const& message);

      BackgroundTaskDeferral taskDeferral = nullptr;

      /// <summary>Win32 Event signaled when the background task is canceled.</summary>
      handle canceledEvent;
   };
}

namespace winrt::BackgroundTaskHost::factory_implementation
{
   struct DartBackgroundTask : DartBackgroundTaskT<DartBackgroundTask, implementation::DartBackgroundTask>
   {
   };
}
