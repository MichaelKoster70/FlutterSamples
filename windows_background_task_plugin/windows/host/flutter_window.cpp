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
#include "flutter/generated_plugin_registrant.h"

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
   RECT frame = GetClientArea();

   // The size here must match the window dimensions to avoid unnecessary surface creation / destruction in the startup path.
   _flutterController = std::make_unique<FlutterViewController>(frame.right - frame.left, frame.bottom - frame.top, _project);
   
   // Ensure that basic setup of the controller was successful.
   if (!_flutterController->engine() || !_flutterController->view())
   {
      return false;
   }

   // Register plugins.
   RegisterPlugins(_flutterController->engine());
   SetChildContent(_flutterController->view()->GetNativeWindow());

   return true;
}

void FlutterWindow::OnDestroy()
{
   if (_flutterController)
   {
      _flutterController = nullptr;
   }
}

LRESULT FlutterWindow::MessageHandler(HWND hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept
{
   // Give Flutter, including plugins, an opportunity to handle window messages.
   if (_flutterController)
   {
      std::optional<LRESULT> result = _flutterController->HandleTopLevelWindowProc(hwnd, message, wparam, lparam);
      if (result)
      {
         return *result;
      }
   }

   return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
