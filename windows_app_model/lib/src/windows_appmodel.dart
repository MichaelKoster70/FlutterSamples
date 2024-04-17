// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
import 'package:windows_appmodel/src/windows_appmodel_bindings.dart';

import 'platform_checker.dart';

/// Interface definition for Windows App Model Support functions
abstract interface class WindowsAppModel {
  /// Factory constructor for the WindowsAppModel.
  ///
  /// Throws an [UnsupportedError] if the current platform is not Windows.
  factory WindowsAppModel() {
    return _WindowsAppModel();
  }

  /// Returns a value whether the running operating system supports the Windows App Model.
  bool isSupported();

  /// Returns a value whether the application is running with an identity.
  bool isRunningWithIdentity();
}

final class _WindowsAppModel implements WindowsAppModel {
  final _bindings = WindowsAppModelBindings(pluginNativeApi);
  _WindowsAppModel() {
    throwIfNotWindows();
  }
  @override
  bool isSupported() => _bindings.isSupported();

  @override
  bool isRunningWithIdentity() => _bindings.isRunningWithIdentity();
}
