// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';

/// Bindings for `windows/client/Win32SyncApis.h`.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
final class WindowsSyncBindings {
  final DynamicLibrary _nativeApi;

  /// The symbols are looked up in [dynamicLibrary].
  WindowsSyncBindings(DynamicLibrary nativeApi) : _nativeApi = nativeApi;

  (int, int) createEvent(bool manualReset, bool initialState, String? name) {
    return using((Arena arena) {
      final Pointer<Utf16> nativeName =
          name != null ? name.toNativeUtf16(allocator: arena) : nullptr;
      final lastError = arena<Uint32>();
      final handle = _win32CreateEvent(
        manualReset ? TRUE : FALSE,
        initialState ? TRUE : FALSE,
        nativeName,
        lastError,
      );

      return (handle, lastError.value);
    });
  }

  (int, int) openEvent(int desiredAccess, bool inheritHandle, String name) {
    return using((Arena arena) {
      final Pointer<Utf16> nativeName = name.toNativeUtf16(allocator: arena);
      final lastError = arena<Uint32>();
      final handle = _win32OpenEvent(
        desiredAccess,
        inheritHandle ? TRUE : FALSE,
        nativeName,
        lastError,
      );

      return (handle, lastError.value);
    });
  }

  late final _win32CreateEvent = _nativeApi.lookupFunction<
      IntPtr Function(
        Int32 bManualReset,
        Int32 bInitialState,
        Pointer<Utf16> lpName,
        Pointer<Uint32> dwLastError,
      ),
      int Function(
        int bManualReset,
        int bInitialState,
        Pointer<Utf16> lpName,
        Pointer<Uint32> dwLastError,
      )>('Win32CreateEvent');

  late final _win32OpenEvent = _nativeApi.lookupFunction<
      IntPtr Function(
        Uint32 dwDesiredAccess,
        Int32 bInheritHandle,
        Pointer<Utf16> lpName,
        Pointer<Uint32> dwLastError,
      ),
      int Function(
        int dwDesiredAccess,
        int bInheritHandle,
        Pointer<Utf16> lpName,
        Pointer<Uint32> dwLastError,
      )>('Win32OpenEvent');
}

/// Dynamic Library instance shared across all types
final pluginNativeApi = DynamicLibrary.open('WindowsSync.dll');
