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

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.ApplicationModel.h"
#include "winrt/Windows.ApplicationModel.AppService.h"
#include "winrt/Windows.System.h"
#include "winrt/Windows.UI.Notifications.h"
#include "winrt/Windows.Data.Xml.Dom.h"

// ----------------------------------------------------------------------------
// Usings
// ----------------------------------------------------------------------------
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::AppService;
using namespace winrt::Windows::System;
using namespace winrt::Windows::UI::Notifications;

// ----------------------------------------------------------------------------
// Class Implementation
// ----------------------------------------------------------------------------
namespace winrt::BackgroundTaskHost::implementation
{
   DartBackgroundTask::DartBackgroundTask()
   {
      //EMPTY_BODY
   }

   void DartBackgroundTask::Run(IBackgroundTaskInstance const& taskInstance)
   {
      DebugLog("NATIVE::UWP_HOST::DartBackgroundTask::Run Enter");

      taskDeferral = taskInstance.GetDeferral();

      // Get the name of the task, use it as identifer on what to call in the dart code
      auto taskName = taskInstance.Task().Name();

      DebugLog("NATIVE::UWP_HOST::DartBackgroundTask::Run: taskName={}", winrt::to_string(taskName));

      DebugShowToast(L"Starting Task: " + taskName);

      taskInstance.Canceled({ this, &DartBackgroundTask::OnCanceled });

      try
      {
         FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync(L"DartUwpBackgroundTask").get();
      }
      catch (const winrt::hresult_error& ex)
      {
         auto hr = ex.code();
         DebugLog("NATIVE::UWP_HOST::DartBackgroundTask::Run: HR={:x}, text={}", hr.value, winrt::to_string(ex.message()));
      }
      catch (...)
      {
         DebugLog("NATIVE::UWP_HOST::DartBackgroundTask::Run: unknown exception");
      }

      // mark the task as completed
      taskDeferral.Complete();
      DebugLog("NATIVE::UWP_HOST::DartBackgroundTask::Run Exit");
   }

   void __stdcall DartBackgroundTask::OnCanceled(_In_ IBackgroundTaskInstance sender, _In_ BackgroundTaskCancellationReason reason)
   {
      DebugLog("NATIVE::UWP_HOST::DartBackgroundTask::OnCanceled");
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
}
