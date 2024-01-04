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
// Consstant
// ----------------------------------------------------------------------------

// Message sent to the main thread to notify that the channel is initialized.
const UINT kNotifyChannelInitialized = WM_USER + 2;
const UINT kExecuteTaskCompleted = WM_USER + 3;

// ----------------------------------------------------------------------------
// Class Implementation
// ----------------------------------------------------------------------------

ComDartBackgroundTask::ComDartBackgroundTask()
   : engineHost(g_project)
{//EMPTY_BODY
}

void __stdcall ComDartBackgroundTask::Run(_In_ IBackgroundTaskInstance taskInstance)
{
   std::cout << "NATIVE::ComDartBackgroundTask::Run" << std::endl;

   // Get the name of the task, use it as identifer on what to call in the dart code
   auto taskName = taskInstance.Task().Name();

   taskInstance.Canceled({ this, &ComDartBackgroundTask::OnCanceled });

   taskDeferral = taskInstance.GetDeferral();

   engineHost.SetNotifyChannelInitializedHandler([this, taskName]() {
      engineHost.PostTaskMessage(kNotifyChannelInitialized, NULL, NULL);
      });

   engineHost.SetTaskMessageHandler([this, taskName](UINT message, WPARAM wParam, LPARAM lParam)
      {
         switch (message)
         {
         case kNotifyChannelInitialized:
            HandleNotifyChannelInitialized(winrt::to_string(taskName));
            return false;

         case kExecuteTaskCompleted:
            HandleExecuteTaskCompleted(wParam);
            return false;

         default:
            return true;
         }
     });

   engineHost.Run(taskName.c_str());

   // mark the task as completed
   taskDeferral.Complete();

   // signal the process exit event
   check_bool(SetEvent(g_processExitEvent.get()));
}

void  ComDartBackgroundTask::OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason)
{
   std::cout << "NATIVE::ComDartBackgroundTask::OnCanceled" << std::endl;
   engineHost.Shutdown();
   isCanceled = true;
}

void ComDartBackgroundTask::HandleNotifyChannelInitialized(const std::string& taskName)
{
   std::cout << "NATIVE::ComDartBackgroundTask::HandleNotifyChannelInitialized()" << std::endl;
   engineHost.GetChannel()->ExecuteTask(taskName, [this](bool result)
      {
         engineHost.PostTaskMessage(kExecuteTaskCompleted, result, NULL);
      });
}

void ComDartBackgroundTask::HandleExecuteTaskCompleted(bool result)
{
   std::cout << "NATIVE::ComDartBackgroundTask::HandleExecuteTaskCompleted()" << std::endl;
   engineHost.Shutdown();
}
