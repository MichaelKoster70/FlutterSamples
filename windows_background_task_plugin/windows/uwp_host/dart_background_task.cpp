// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "pch.h"
#include "dart_background_task.h"
#include "DartBackgroundTask.g.cpp"

// ----------------------------------------------------------------------------
// Externals
// ----------------------------------------------------------------------------
extern flutter::DartProject g_project;

// ----------------------------------------------------------------------------
// Constant
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Class Implementation
// ----------------------------------------------------------------------------
namespace winrt::BackgroundTaskHost::implementation
{
   DartBackgroundTask::DartBackgroundTask()
      : engineHost(g_project)
   {
      //EMPTY_BODY
   }

   void DartBackgroundTask::Run(IBackgroundTaskInstance const& taskInstance)
   {
      std::cout << "NATIVE::ComDartBackgroundTask::Run" << std::endl;

      // Get the name of the task, use it as identifer on what to call in the dart code
      auto taskName = taskInstance.Task().Name();

      taskInstance.Canceled({ this, &DartBackgroundTask::OnCanceled });
   }

   void __stdcall DartBackgroundTask::OnCanceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
   {
   }
}
