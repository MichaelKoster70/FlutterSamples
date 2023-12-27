// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "flutter_engine_host.h"
#include <optional>
#include "flutter/generated_plugin_registrant.h"

// ----------------------------------------------------------------------------
// Implementation
// ----------------------------------------------------------------------------
FlutterEngineHost::FlutterEngineHost(const flutter::DartProject& project)
   : _window(project) {}

FlutterEngineHost::~FlutterEngineHost()
{
   //EMPTY_BODY
}

bool FlutterEngineHost::Run(const std::wstring& title, bool exitProcessOnEngineShutdown)
{
   auto success = _window.Create(title);
   if (success)
   {
      _window.SetQuitOnClose(exitProcessOnEngineShutdown);
   }
   return success;
}

void FlutterEngineHost::Shutdown()
{
   _window.Destroy();
}
