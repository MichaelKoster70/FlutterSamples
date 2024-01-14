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
#include "flutter_window.h"

// ----------------------------------------------------------------------------
// Class declarations
// ----------------------------------------------------------------------------

using ShutdownHandler = std::function<void()>;

using TaskMessageHandlerFunction = std::function<bool(UINT const message, WPARAM const wParam, LPARAM const lParam)>;

/// <summary>
/// Class proividing a host for a Flutter engine minimal Window support.
/// </summary>
/// <remarks>
/// This class is a minimal implementation of the Flutter engine host.
/// Once Flutter supports true headless operation, this class will provide the required plumbing.
class FlutterEngineHost
{
public:
   /// <summary>
   /// Creates a new host for the given project.
   /// </summary>
   /// <param name="project">The project holding the configuration.</param>
   explicit FlutterEngineHost(const flutter::DartProject& project);
   virtual ~FlutterEngineHost();

   /// <summary>
   /// Sets the handler for the NotifyChannelInitialized method called by flutter.
   /// </summary>
   /// <param name="handler">The handler.</param>
   void SetNotifyChannelInitializedHandler(NotifyChannelInitializedHandler handler);

   /// <summary>
   /// Posts a Windows message to the main platform thread.
   /// </summary>
   /// <param name="message">The message to post.</param>
   /// <param name="wParam">Message specific parameters.</param>
   /// <param name="lParam">Message specific parameters.</param>
   void PostTaskMessage(UINT const message, WPARAM const wParam, LPARAM const lParam);

   /// <summary>
   /// Sets the handler processing task messages.
   /// </summary>
   /// <param name="handler"></param>
   void SetTaskMessageHandler(TaskMessageHandlerFunction handler);

   /// <summary>
   /// Creates a minimal view and initializes the Flutter engine.
   /// </summary>
   /// <param name="title">The title of the window.</param>
   /// <returns>The exit code.</returns>
   int Run(const std::wstring& title);

   std::shared_ptr<FlutterMethodChannel> GetChannel() const { return _window.GetChannel(); }

   /// <summary>
   /// Stops the Flutter engine and destroys the window.
   /// </summary>
   void Shutdown();

private:
   bool HandleThreadMessage(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;

   DWORD _platformThreadId = 0;
   FlutterWindow _window;
   TaskMessageHandlerFunction _taskMessageHandler;
   NotifyChannelInitializedHandler _notifyChannelInitializedHandler;
};
