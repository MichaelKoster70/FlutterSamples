// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'package:win32/win32.dart';
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

  /// Registers a timer tigger based background task with the [taskName] and [freshnessTime] in minutes.
  /// If [oneShot] is true, the background task will only be triggered once.
  /// The [freshnessTime] must be between 15 minutes and 30 days.
  ///
  /// Throws an [WindowsUniversalException] if the registration fails.
  void registerComBackgroundTaskTimer(
      String taskName, int freshnessTime, bool oneShot);

  /// Unregisters the background task with the [taskName].
  void unregisterBackgroundTask(String taskName);

  /// Uninitializes the background task client.
  /// This function must be called after all other functions in this library.
  /// This function will uninitialite the Windows Runtime COM infrastructure.
  void uninitialize();
}

/// The ClassID of the COM class implementing the background task.
final _backgroundTaskClassid =
    Guid.parse('88F46315-0A89-4911-9199-6BB496C14533');

final class _WindowsBackgroundTaskClient
    implements WindowsBackgroundTaskClient {
  final _bindings = WindowsBackgroundTaskClientBindings(pluginNativeApi);

  @override
  void initialize(String applicationId) {
    final hr = _bindings.initialize(applicationId);
    if (hr != 0) {
      throw WindowsUniversalException('Failed to initialize', hr);
    }
  }

  @override
  void registerComBackgroundTaskTimer(
      String taskName, int freshnessTime, bool oneShot) {
    if (freshnessTime < 15 || freshnessTime > 30 * 24 * 60) {
      throw const WindowsUniversalException(
          'The freshnessTime must be between 15 minutes and 30 days', 0);
    }

    final hr = _bindings.registerComBackgroundTaskTimer(
        _backgroundTaskClassid, taskName, freshnessTime, oneShot);

    if (hr != 0) {
      throw WindowsUniversalException(
          'Failed to register the Windows Background Task', hr);
    }
  }

  @override
  void unregisterBackgroundTask(String taskName) {
    final hr = _bindings.unregisterBackgroundTask(taskName);
    if (hr != 0) {
      throw WindowsUniversalException(
          'Failed to unregister the Windows Background Task', hr);
    }
  }

  @override
  void uninitialize() {
    _bindings.uninitialize();
  }
}
