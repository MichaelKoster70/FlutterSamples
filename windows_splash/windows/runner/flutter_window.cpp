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
// Class implementations
// ----------------------------------------------------------------------------

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
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
   // Show the splash screen while the Flutter engine is starting up.
   _splashScreen.Create(*this);

   RECT frame = GetClientArea();

   // The size here must match the window dimensions to avoid unnecessary surface
   // creation / destruction in the startup path.
   _flutterController = std::make_unique<flutter::FlutterViewController>(frame.right - frame.left, frame.bottom - frame.top, _project);
   // Ensure that basic setup of the controller was successful.
   if (!_flutterController->engine() || !_flutterController->view())
   {
      return false;
   }

   RegisterPlugins(_flutterController->engine());
   SetChildContent(_flutterController->view()->GetNativeWindow());

   _flutterController->engine()->SetNextFrameCallback([&]()
      {
         this->Show();
         _splashScreen.Destroy();
      });

   // Flutter can complete the first frame before the "show window" callback is
   // registered. The following call ensures a frame is pending to ensure the
   // window is shown. It is a no-op if the first frame hasn't completed yet.
   _flutterController->ForceRedraw();

   return true;
}

void FlutterWindow::OnDestroy()
{
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

   switch (message)
   {
   case WM_FONTCHANGE:
      _flutterController->engine()->ReloadSystemFonts();
      break;
   }

   return Win32Window::MessageHandler(hWnd, message, wParam, lParam);
}
