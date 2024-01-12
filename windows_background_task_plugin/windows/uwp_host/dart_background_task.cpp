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
#include "debug_log.h"
#include "DartBackgroundTask.g.cpp"

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>

// ----------------------------------------------------------------------------
// Usings
// ----------------------------------------------------------------------------
using namespace winrt::Windows::UI::Notifications;

// ----------------------------------------------------------------------------
// Externals
// ----------------------------------------------------------------------------
extern flutter::DartProject g_project;

// ----------------------------------------------------------------------------
// Consstant
// ----------------------------------------------------------------------------

// Message sent to the main thread to notify that the channel is initialized.
const UINT kNotifyChannelInitialized = WM_USER + 2;

// Message sent to the main thread to notify that the task is completed.
const UINT kExecuteTaskCompleted = WM_USER + 3;

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
      DebugLog("NATIVE::DartBackgroundTask::Run");

      // Get the name of the task, use it as identifer on what to call in the dart code
      auto taskName = taskInstance.Task().Name();

      DebugShowToast(L"Starting Task: " + taskName);

      taskInstance.Canceled({ this, &DartBackgroundTask::OnCanceled });

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

      DebugLog("NATIVE::DartBackgroundTask::Run -before engineHost.Run");
      engineHost.Run(taskName.c_str());
      DebugLog("NATIVE::DartBackgroundTask::Run -after  engineHost.Run");

      // mark the task as completed
      taskDeferral.Complete();
   }

   void __stdcall DartBackgroundTask::OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason)
   {
      DebugLog("NATIVE::DartBackgroundTask::OnCanceled");
      engineHost.Shutdown();
   }

   void DartBackgroundTask::DebugShowToast(hstring const& message)
   {
      // Create a toast notification with the message showing a single line of text
      auto toastXml = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastText02);

      // fill text
      auto toastTextElements = toastXml.GetElementsByTagName(L"text");
      toastTextElements.Item(0).AppendChild(toastXml.CreateTextNode(L"DEBUG:: DartBackgroundTask"));
      toastTextElements.Item(1).AppendChild(toastXml.CreateTextNode(message));

      // and show
      auto toast = ToastNotification(toastXml);
      ToastNotificationManager::CreateToastNotifier().Show(toast);
   }

   void DartBackgroundTask::HandleNotifyChannelInitialized(const std::string& taskName)
   {
      DebugLog("NATIVE::ComDartBackgroundTask::HandleNotifyChannelInitialized");
      engineHost.GetChannel()->ExecuteTask(taskName, [this](bool result)
         {
            engineHost.PostTaskMessage(kExecuteTaskCompleted, result, NULL);
         });
   }

   void DartBackgroundTask::HandleExecuteTaskCompleted(bool result)
   {
      DebugLog("NATIVE::ComDartBackgroundTask::HandleExecuteTaskCompleted");
      engineHost.Shutdown();
   }
}
