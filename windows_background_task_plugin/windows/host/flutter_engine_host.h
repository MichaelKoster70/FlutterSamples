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

  static void SetMainThread(DWORD mainThreadId) { _mainThreadId = mainThreadId; }

  static LRESULT ThreadMessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept;

  /// <summary>
  /// Sets the handler for the NotifyChannelInitialized method called by flutter.
  /// </summary>
  /// <param name="handler">The handler.</param>
  void SetNotifyChannelInitializedHandler(NotifyChannelInitializedHandler handler);

  /// <summary>
  /// Sets the handler to be called when the engine shuts down.
  /// </summary>
  /// <param name="handler">The handler delegate</param>
  void SetShutdownHandler(ShutdownHandler handler);

  void SendRunMessage(const std::wstring& title);

  /// <summary>
  /// Creates a minimal view and initializes the Flutter engine.
  /// </summary>
  /// <param name="title">The title of the window.</param>
  /// <returns>true if the engine started successfully.</returns>
  bool Run(const std::wstring& title);

  /// <summary>
  /// Gets the channel used to communicate with the Flutter engine.
  /// </summary>
  /// <returns>The channel</returns>
  std::shared_ptr<FlutterMethodChannel> GetChannel() const { return _window.GetChannel(); }

  /// <summary>
  /// Stops the Flutter engine and destroys the window.
  /// </summary>
  void Shutdown();

 private:
   static DWORD _mainThreadId;
   FlutterWindow _window;
   std::wstring _title;
   NotifyChannelInitializedHandler _notifyChannelInitializedHandler;
   ShutdownHandler _shutdownHandler;
};
