// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Holds all APIs for TimeTrigger based background tasks
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "DllExports.h"
#include <windows.h>
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.ApplicationModel.Background.h"
#include "BackgroundTaskClientApi.h"
#include "BackgroundTaskHelper.h"


// ----------------------------------------------------------------------------
// namespace usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::ApplicationModel::Background;

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------
static bool g_backgroundAccessRequested = false;

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
void RequestBackgroundAccess();
void RemoveBackgroundAccess();

// ----------------------------------------------------------------------------
// API Implementation
// ----------------------------------------------------------------------------

HRESULT RegisterComBackgroundTaskTimer
(
   _In_ GUID* taskClassId,
   _In_ LPCWSTR taskName,
   _In_ UINT32 freshnessTime,
   _In_ BOOL oneShot
)
{
   try
   {
      if (taskName == nullptr || taskClassId == nullptr)
      {
         return E_INVALIDARG;
      };

      RequestBackgroundAccess();

      auto trigger = TimeTrigger(freshnessTime, oneShot);
      auto classId = guid(*taskClassId);
      BackgroundTaskHelper::Register(taskName, classId, trigger);

      return S_OK;
   }
   catch (...)
   {
      return to_hresult();
   }
}

// function to request access to background tasks, throws an exception if access is denied
void RequestBackgroundAccess()
{
   // 
   if (g_backgroundAccessRequested)
   {
      return;
   }

   auto status = BackgroundExecutionManager::RequestAccessAsync().get();

   if (status == BackgroundAccessStatus::AlwaysAllowed || status == BackgroundAccessStatus::AllowedSubjectToSystemPolicy)
   {
      g_backgroundAccessRequested = true;
      return;
   }
   else if (status == BackgroundAccessStatus::DeniedBySystemPolicy || status == BackgroundAccessStatus::DeniedByUser)
   {
      throw_hresult(E_ACCESSDENIED);
   }
   else
   {
      throw_hresult(E_FAIL);
   }
}

void RemoveBackgroundAccess()
{
   g_backgroundAccessRequested = false;

   BackgroundExecutionManager::RemoveAccess();
}