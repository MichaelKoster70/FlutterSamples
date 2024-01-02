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
#include <iostream>

// ----------------------------------------------------------------------------
// Externals
// ----------------------------------------------------------------------------
extern flutter::DartProject g_project;

extern handle g_processExitEvent;

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

   engineHost.SetNotifyChannelInitializedHandler([this, taskName]() {
      std::cout << "ComDartBackgroundTask::Run - Channel Initialized" << std::endl;
      engineHost.GetChannel()->ExecuteTask(winrt::to_string(taskName));
   });

   engineHost.SetShutdownHandler([this]() {
      std::cout << "ComDartBackgroundTask::Run - Shutdown" << std::endl;
      taskDeferral.Complete();
   });

   engineHost.Run(taskName.c_str());

   check_bool(SetEvent(g_processExitEvent.get()));

}

void  ComDartBackgroundTask::OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason)
{
   std::cout << "ComDartBackgroundTask::OnCanceled" << std::endl;
   isCanceled = true;
}
