// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Helper class for Windows Background Task Registration
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.ApplicationModel.Background.h"
#include "BackgroundTaskHelper.h"

// ----------------------------------------------------------------------------
// Namespace usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::ApplicationModel::Background;

// ----------------------------------------------------------------------------
// Class Implementation
// ----------------------------------------------------------------------------

IBackgroundTaskRegistration BackgroundTaskHelper::RegisterAndGetRegistration(hstring taskName, guid classId, IBackgroundTrigger trigger, IVector<IBackgroundCondition> conditions)
{
   // Check if the task is already registered.
   auto registration = GetTaskRegistration(taskName);
   if (registration != nullptr)
   {
      return registration;
   }

   // Register the background task with name and trigger.
   auto builder = BackgroundTaskBuilder();
   builder.Name(taskName);
   builder.SetTrigger(trigger);
   builder.SetTaskEntryPointClsid(classId);
   registration = builder.Register();

   return registration;
}

void BackgroundTaskHelper::Register(hstring taskName, guid classId, IBackgroundTrigger trigger, IVector<IBackgroundCondition> conditions)
{
   // Check if the task is already registered.
   if (IsTaskRegistered(taskName))
   {
      return;
   }

   // Register the background task with name and trigger.
   auto builder = BackgroundTaskBuilder();
   builder.Name(taskName);
   builder.SetTrigger(trigger);
   for (auto condition : conditions)
   {
      builder.AddCondition(condition);
   }
   builder.SetTaskEntryPointClsid(classId);
   builder.Register();
}

void BackgroundTaskHelper::Register(hstring taskName, hstring taskEntryPoint, IBackgroundTrigger trigger, IVector<IBackgroundCondition> conditions)
{
   // Check if the task is already registered.
   if (IsTaskRegistered(taskName))
   {
      return;
   }

   // Register the background task with name and trigger.
   auto builder = BackgroundTaskBuilder();
   builder.Name(taskName);
   builder.SetTrigger(trigger);
   for (auto condition : conditions)
   {
      builder.AddCondition(condition);
   }
   builder.TaskEntryPoint(taskEntryPoint);
   builder.Register();
}

void BackgroundTaskHelper::Unregister(hstring taskName)
{
   // Unregister the background task with name.
   auto registration = GetTaskRegistration(taskName);
   if (registration != nullptr)
   {
      registration.Unregister(true);
   }
}

void BackgroundTaskHelper::UnregisterAll()
{
   // Iterate over all background tasks and unregister them.
   auto registeredTasks = BackgroundTaskRegistration::AllTasks();
   for (auto iter = registeredTasks.begin(); iter != registeredTasks.end(); iter++)
   {
      auto registration = iter.Current().Value();
      registration.Unregister(true);
   }
}

bool BackgroundTaskHelper::IsTaskRegistered(hstring taskName)
{
   // Iterate over all background tasks and check if the task is already registered.
   auto registeredTasks = BackgroundTaskRegistration::AllTasks();
   for (auto iter = registeredTasks.begin(); iter != registeredTasks.end(); iter++)
   {
      if (iter.Current().Value().Name() == taskName)
      {
         return true;
      }
   }

   return false;
}

IBackgroundTaskRegistration BackgroundTaskHelper::GetTaskRegistration(hstring taskName)
{
   // Iterate over all background tasks and check if the task is already registered.
   auto registeredTasks = BackgroundTaskRegistration::AllTasks();
   for (auto iter = registeredTasks.begin(); iter != registeredTasks.end(); iter++)
   {
      auto registration = iter.Current().Value();
      if (registration.Name() == taskName)
      {
         return registration;
      }
   }

   return nullptr;
}
