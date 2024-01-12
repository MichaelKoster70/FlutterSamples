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

int FlutterEngineHost::Run(const std::wstring& title)
{
   auto success = _window.Create(title);

   if (!success)
   {
      return EXIT_FAILURE;
   }

   _window.SetQuitOnClose(true);

   // run the main message loop
   ::MSG msg;
   while (::GetMessage(&msg, nullptr, 0, 0))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   return EXIT_SUCCESS;
}
