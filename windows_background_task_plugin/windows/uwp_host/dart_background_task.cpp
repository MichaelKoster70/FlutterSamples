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

   void DartBackgroundTask::Run(winrt::Windows::ApplicationModel::Background::IBackgroundTaskInstance const& taskInstance)
   {
      throw hresult_not_implemented();
   }
}
