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
// Consstant
// ----------------------------------------------------------------------------
const UINT kEngineRun = WM_USER + 1;

// ----------------------------------------------------------------------------
// Implementation
// ----------------------------------------------------------------------------

DWORD FlutterEngineHost::_mainThreadId = 0;

FlutterEngineHost::FlutterEngineHost(const flutter::DartProject& project)
   : _window(project) {}

FlutterEngineHost::~FlutterEngineHost()
{
   //EMPTY_BODY
}

LRESULT FlutterEngineHost::ThreadMessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
{
   if (message == kEngineRun)
   {
      auto engineHost = reinterpret_cast<FlutterEngineHost*>(wparam);
      auto title = reinterpret_cast<const wchar_t*>(lparam);
      engineHost->Run(title);
      return 0;
   }

   return 1;
}

bool FlutterEngineHost::Run(const std::wstring& title)
{
   auto success = _window.Create(title);
   if (success)
   {
      _window.SetQuitOnClose(true);
      
      if (_notifyChannelInitializedHandler)
      {
         _window.GetChannel()->SetNotifyChannelInitializedHandler(_notifyChannelInitializedHandler);
      }

      _window.SetDestroyHandler([this]() {
         if (_shutdownHandler)
         {
            _shutdownHandler();
         }
      });

      // run the main message loop
      ::MSG msg;
      while (::GetMessage(&msg, nullptr, 0, 0))
      {
         ::TranslateMessage(&msg);
         ::DispatchMessage(&msg);
      }
   }

   return success;
}

void FlutterEngineHost::SetNotifyChannelInitializedHandler(NotifyChannelInitializedHandler handler)
{
   _notifyChannelInitializedHandler = handler;
}

void FlutterEngineHost::SetShutdownHandler(ShutdownHandler handler)
{
   _shutdownHandler = handler;
}

void FlutterEngineHost::SendRunMessage(const std::wstring& title)
{
   _title = title;

   PostThreadMessage(_mainThreadId, kEngineRun, reinterpret_cast<WPARAM>(this), reinterpret_cast<LPARAM>(_title.c_str()));
}

void FlutterEngineHost::Shutdown()
{
   _window.Destroy();
}
