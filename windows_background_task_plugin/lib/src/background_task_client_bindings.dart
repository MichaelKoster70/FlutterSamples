// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';

/// Bindings for `windows/client/BackgroundTaskClientApi.h`.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
final class WindowsBackgroundTaskClientBindings {
  final DynamicLibrary _nativeApi;

  /// The symbols are looked up in [dynamicLibrary].
  WindowsBackgroundTaskClientBindings(DynamicLibrary nativeApi)
      : _nativeApi = nativeApi;

  /// Initializes the background task client with the [applicationId].
  /// Returns 0 if the initialization succeeds.
  int initialize(String applicationId) {
    int result = 0;
    using((Arena arena) {
      final Pointer<Utf16> nativeApplicationId =
          applicationId.toNativeUtf16(allocator: arena);
      result = _initialize(nativeApplicationId);
    });
    return result;
  }

  int registerComBackgroundTaskTimer(
      Guid taskClassId, String taskName, int freshnessTime, bool oneShot) {
    int result = 0;
    using((Arena arena) {
      final Pointer<Utf16> nativeTaskName =
          taskName.toNativeUtf16(allocator: arena);
      final nativeTaskClassId = taskClassId.toNativeGUID(allocator: arena);
      result = _registerComBackgroundTaskTimer(nativeTaskClassId,
          nativeTaskName, freshnessTime, oneShot ? TRUE : FALSE);
    });
    return result;
  }

  int unregisterBackgroundTask(String taskName) {
    int result = 0;
    using((Arena arena) {
      final Pointer<Utf16> nativeTaskName =
          taskName.toNativeUtf16(allocator: arena);
      result = _unregisterBackgroundTask(nativeTaskName);
    });
    return result;
  }

  /// Uninitializes the background task client.
  void uninitialize() {
    _uninitialize();
  }

  late final _initialize = _nativeApi.lookupFunction<
      Uint32 Function(Pointer<Utf16>),
      int Function(Pointer<Utf16>)>('Initialize');

  late final _registerComBackgroundTaskTimer = _nativeApi.lookupFunction<
      Uint32 Function(Pointer<GUID>, Pointer<Utf16>, Uint32, Uint32),
      int Function(Pointer<GUID>, Pointer<Utf16>, int,
          int)>('RegisterComBackgroundTaskTimer');

  late final _unregisterBackgroundTask = _nativeApi.lookupFunction<
      Uint32 Function(Pointer<Utf16>),
      int Function(Pointer<Utf16>)>('UnregisterBackgroundTask');

  late final _uninitialize = _nativeApi
      .lookupFunction<Void Function(), void Function()>('Uninitialize');
}
