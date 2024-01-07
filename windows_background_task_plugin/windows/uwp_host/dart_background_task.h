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
#include "flutter_engine_host.h"

// ----------------------------------------------------------------------------
// Usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::ApplicationModel::Background;

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------

namespace winrt::BackgroundTaskHost::implementation
{
   struct DartBackgroundTask : DartBackgroundTaskT<DartBackgroundTask>
   {
      DartBackgroundTask();

      void Run(IBackgroundTaskInstance const& taskInstance);

   private:
      FlutterEngineHost engineHost;
      BackgroundTaskDeferral taskDeferral = nullptr;
   };
}

namespace winrt::BackgroundTaskHost::factory_implementation
{
   struct DartBackgroundTask : DartBackgroundTaskT<DartBackgroundTask, implementation::DartBackgroundTask>
   {
   };
}
