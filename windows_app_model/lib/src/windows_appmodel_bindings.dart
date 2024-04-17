// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
import 'dart:ffi';

/// Bindings for `windows/AppModelHelper.h`.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
final class WindowsAppModelBindings {
  final DynamicLibrary _nativeApi;

  /// The symbols are looked up in [dynamicLibrary].
  WindowsAppModelBindings(DynamicLibrary nativeApi) : _nativeApi = nativeApi;

  bool isSupported() {
    return _appModelIsSupported();
  }

  bool isRunningWithIdentity() {
    return _appModelIsRunningWithIdentity();
  }

  late final _appModelIsSupported = _nativeApi
      .lookupFunction<Bool Function(), bool Function()>('AppModelIsSupported');

  late final _appModelIsRunningWithIdentity =
      _nativeApi.lookupFunction<Bool Function(), bool Function()>(
          'AppModelIsRunningWithIdentity');
}

/// Dynamic Library instance shared across all types
final pluginNativeApi = DynamicLibrary.open('WindowsAppModel.dll');
