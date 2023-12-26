// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:ffi';
import 'package:ffi/ffi.dart';

/// Bindings for `windows/client/BackgroundTaskClientApi.h.h`.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
final class WindowsBackgroundTaskClientBindings {
  final F Function<T extends Function, F extends Function>(String symbolName)
      _lookupFunction;

  /// The symbols are looked up in [dynamicLibrary].
  WindowsBackgroundTaskClientBindings(DynamicLibrary nativeApi)
      : _lookupFunction = nativeApi.lookupFunction;

  /// Initializes the background task client with the [applicationId].
  /// Returns 0 if the initialization succeeds.
  int initialize(String applicationId) {
    final Pointer<Utf16> applicationIdPtr = applicationId.toNativeUtf16();
    try {
      return _initialize(applicationIdPtr);
    } finally {
      calloc.free(applicationIdPtr);
    }
  }

  void uninitialize() {
    _uninitialize();
  }

  late final _initialize = _lookupFunction<Uint32 Function(Pointer<Utf16>),
      int Function(Pointer<Utf16>)>('initialize');

  late final _uninitialize =
      _lookupFunction<Void Function(), void Function()>('Uninitialize');
}
