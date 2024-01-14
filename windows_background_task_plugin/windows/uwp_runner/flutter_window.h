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
#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <memory>
#include "win32_window.h"
#include "flutter_method_channel.h"

// ----------------------------------------------------------------------------
// namespace usings
// ----------------------------------------------------------------------------
using namespace flutter;

// ----------------------------------------------------------------------------
// Class definition
// ----------------------------------------------------------------------------

/// <summary>
/// A Win32 window that hosts a Flutter view and its engine.
/// </summary>
class FlutterWindow : public Win32Window
{
public:
   // Creates a new FlutterWindow hosting a Flutter view running |project|.
   explicit FlutterWindow(const DartProject& project);
   virtual ~FlutterWindow();

   /// <summary>
   /// Returns the channel used to communicate with the Flutter engine.
   /// </summary>
   /// <returns>Shared pointer holding the channel.</returns>
   std::shared_ptr<FlutterMethodChannel> GetChannel() const { return _channel; }

protected:
   // Win32Window:
   bool OnCreate() override;
   void OnDestroy() override;
   LRESULT MessageHandler(HWND window, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept override;

private:
   // The project to run.
   DartProject _project;

   // The Flutter instance hosted by this window.
   std::unique_ptr<FlutterViewController> _flutterController;

   // The channel used to communicate with the Flutter engine.
   std::shared_ptr<FlutterMethodChannel> _channel;
};

