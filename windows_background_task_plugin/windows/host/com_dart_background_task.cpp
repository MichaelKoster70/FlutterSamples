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
#include "com_dart_background_task.h"
#include "winrt/Windows.ApplicationModel.ExtendedExecution.h"
#include <iostream>


// ----------------------------------------------------------------------------
// Externals
// ----------------------------------------------------------------------------
extern flutter::DartProject g_project;

// ----------------------------------------------------------------------------
// Class Implementation
// ----------------------------------------------------------------------------

ComDartBackgroundTask::ComDartBackgroundTask()
   : engineHost(g_project)
{//EMPTY_BODY
}

void __stdcall ComDartBackgroundTask::Run(_In_ IBackgroundTaskInstance taskInstance)
{
   std::cout << "ComDartBackgroundTask::Run" << std::endl;

   // Get the name of the task, use it as identifer on what to call in the dart code
   auto taskName = taskInstance.Task().Name();

   taskInstance.Canceled({ this, &ComDartBackgroundTask::OnCanceled });

   taskDeferral = taskInstance.GetDeferral();

   taskDeferral.Complete();
}

void  ComDartBackgroundTask::OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason)
{
   std::cout << "ComDartBackgroundTask::OnCanceled" << std::endl;
   isCanceled = true;
}
