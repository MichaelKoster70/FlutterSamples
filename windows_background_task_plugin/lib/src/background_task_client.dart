// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'package:windows_background_task_plugin/src/background_task_client_bindings.dart';
import 'package:windows_background_task_plugin/src/plugin_native_apis.dart';
import 'package:windows_background_task_plugin/src/windows_exception.dart';

/// Interface defining the client interfacing with the Windows UWP based background task infrastructure.
abstract interface class WindowsBackgroundTaskClient {
  /// Factory constructor for the WindowsBackgroundTaskClient.
  factory WindowsBackgroundTaskClient() {
    return _WindowsBackgroundTaskClient();
  }

  /// Initializes the background task client with the [applicationId].
  ///
  /// This function must be called before any other function in this library.
  /// This function will initialite the Windows Runtime COM infrastructure in multithreaded apartment mode.
  /// Throws an [WindowsUniversalException] if the initialization fails.
  void initialize(String applicationId);

  /// Uninitializes the background task client.
  /// This function must be called after all other functions in this library.
  /// This function will uninitialite the Windows Runtime COM infrastructure.
  void uninitialize();
}

final class _WindowsBackgroundTaskClient
    implements WindowsBackgroundTaskClient {
  final _bindings = WindowsBackgroundTaskClientBindings(pluginNativeApi);

  @override
  void initialize(String applicationId) {
    final hr = _bindings.initialize(applicationId);
    if (hr != 0) {
      throw WindowsUniversalException(
          'Failed to initialize the Windows Background Task Client', hr);
    }
  }

  @override
  void uninitialize() {
    _bindings.uninitialize();
  }
}
