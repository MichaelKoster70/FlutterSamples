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
  /// Creates a minimal view and initializes the Flutter engine.
  /// </summary>
  /// <param name="title">The title of the window.</param>
  /// <param name="exitProcessOnEngineShutdown">Whether to exit the process when the engine shuts down.</param>
  /// <returns>true if the engine started successfully.</returns>
  bool Run(const std::wstring& title, bool exitProcessOnEngineShutdown);

  /// <summary>
  /// Stops the Flutter engine and destroys the window.
  /// </summary>
  void Shutdown();

 private:
   FlutterWindow _window;
};
