// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "flutter_window.h"
#include <optional>
#include <memory>
#include "flutter/generated_plugin_registrant.h"
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>

// ----------------------------------------------------------------------------
// Implementations
// ----------------------------------------------------------------------------

FlutterWindow::FlutterWindow(const DartProject& project)
    : _project(project)
{
   //EMPTY_BODY
}

FlutterWindow::~FlutterWindow()
{
   //EMPTY_BODY
}

bool FlutterWindow::OnCreate()
{

   // The size here must match the window dimensions to avoid unnecessary surface creation / destruction in the startup path.
   _flutterController = std::make_unique<FlutterViewController>(0, 0, _project);
   
   // Ensure that basic setup of the controller was successful.
   if (!_flutterController->engine() || !_flutterController->view())
   {
      return false;
   }

   // Register plugins.
   RegisterPlugins(_flutterController->engine());

   // Set-up a MethodChannel to handle calls from/to Flutter.
   _channel = std::make_shared<FlutterMethodChannel>(_flutterController->engine()->messenger(), "windows.universal/background_task");

   SetChildContent(_flutterController->view()->GetNativeWindow());

   return true;
}

void FlutterWindow::OnDestroy()
{
   if (_channel)
   {
      _channel = nullptr;
   }
   if (_flutterController)
   {
      _flutterController = nullptr;
   }
}

LRESULT FlutterWindow::MessageHandler(HWND hWnd, UINT const message, WPARAM const wParam, LPARAM const lParam) noexcept
{
   // Give Flutter, including plugins, an opportunity to handle window messages.
   if (_flutterController)
   {
      std::optional<LRESULT> result = _flutterController->HandleTopLevelWindowProc(hWnd, message, wParam, lParam);
      if (result)
      {
         return *result;
      }
   }

   return Win32Window::MessageHandler(hWnd, message, wParam, lParam);
}
