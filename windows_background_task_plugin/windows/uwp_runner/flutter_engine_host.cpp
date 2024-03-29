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
#include "debug_log.h"
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

void FlutterEngineHost::SetTaskMessageHandler(TaskMessageHandlerFunction handler)
{
   _taskMessageHandler = handler;
}

void FlutterEngineHost::PostTaskMessage(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
   ::PostThreadMessage(_platformThreadId, message, wParam, lParam);
}

int FlutterEngineHost::Run(const std::wstring& title)
{
   auto success = _window.Create(title);

   if (success)
   {
      _window.SetQuitOnClose(true);
      _platformThreadId = ::GetCurrentThreadId();

      if (_notifyChannelInitializedHandler)
      {
         _window.GetChannel()->SetNotifyChannelInitializedHandler(_notifyChannelInitializedHandler);
      }

      // run the main message loop
      ::MSG msg;
      while (::GetMessage(&msg, nullptr, 0, 0))
      {
         if (HandleThreadMessage(msg.message, msg.wParam, msg.lParam))
         {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
         }
      }
   }

   return success ? EXIT_SUCCESS : EXIT_SUCCESS;
}

void FlutterEngineHost::SetNotifyChannelInitializedHandler(NotifyChannelInitializedHandler handler)
{
   _notifyChannelInitializedHandler = handler;
}

void FlutterEngineHost::Shutdown()
{
   _window.Destroy();
}

bool FlutterEngineHost::HandleThreadMessage(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
{
   if (_taskMessageHandler)
   {
      return _taskMessageHandler(message, wparam, lparam);
   }
   return true;
}
