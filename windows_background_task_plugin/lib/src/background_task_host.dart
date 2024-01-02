// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'package:flutter/services.dart';
import 'package:flutter/widgets.dart';

typedef BackgroundTaskHandler = Future<bool> Function(String taskName);

/// Interface providing the runtime environment for a background task host.
abstract interface class WindowsBackgroundTaskHost {
  factory WindowsBackgroundTaskHost() => _WindowsBackgroundTaskHost();

  /// Have the host execute the background task with the given name.
  ///
  /// Thge VM entrypoint called by the native infrastructure expects this
  /// method to be called to execute the background task.
  void executeTask(final BackgroundTaskHandler backgroundTask);
}

class _WindowsBackgroundTaskHost implements WindowsBackgroundTaskHost {
  _WindowsBackgroundTaskHost();

  @override
  void executeTask(final BackgroundTaskHandler backgroundTask) async {
    WidgetsFlutterBinding.ensureInitialized();
    // Setup the method channel
    _backgroundChannel.setMethodCallHandler((call) async {
      if (call.method == 'executeTask') {
        final taskName = call.arguments as String;
        final result = await backgroundTask(taskName);
        return result;
      }
    });

    // Notify the native host that the channel is initialized
    // Once initialized, the native host will call the executeTask method
    _backgroundChannel.invokeMethod('notifyChannelInitialized');
  }
}

// The MethodChannel used to communicate with the native background task host
MethodChannel _backgroundChannel =
    const MethodChannel("windows.universal/background_task");
