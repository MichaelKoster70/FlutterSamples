// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';
import 'package:windows_event_log/src/event_log_provider.dart';

/// Bindings for `winevt.h` wrappers.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
final class WindowsEventLogProviderBindings {
  final DynamicLibrary _nativeApi;

  WindowsEventLogProviderBindings(this._nativeApi);

  (int, int) eventRegister(Guid providerId) {
    return using((Arena arena) {
      final nativeProviderId = providerId.toNativeGUID(allocator: arena);
      final nativeRegHandle = arena<Uint64>();

      final status =
          _EventRegister(nativeProviderId, nullptr, nullptr, nativeRegHandle);
      if (status != ERROR_SUCCESS) {
        throw WindowsException(HRESULT_FROM_WIN32(status));
      }
      return (nativeRegHandle.value, status);
    });
  }

  void eventWriteNoData(int regHandle, EventDescriptor descriptor) {
    using((Arena arena) {
      final nativeDescriptor = descriptor.toNative(allocator: arena);
      final status = _EventWriteNoData(regHandle, nativeDescriptor);
      if (status != ERROR_SUCCESS) {
        throw WindowsException(HRESULT_FROM_WIN32(status));
      }
    });
  }

  void eventWriteWithInt64(
    int regHandle,
    EventDescriptor descriptor,
    int data,
  ) {
    using((Arena arena) {
      final nativeDescriptor = descriptor.toNative(allocator: arena);
      final status = _EventWriteWithInt64(
        regHandle,
        nativeDescriptor,
        data,
      );
      if (status != ERROR_SUCCESS) {
        throw WindowsException(HRESULT_FROM_WIN32(status));
      }
    });
  }

  void eventWriteWithString(
    int regHandle,
    EventDescriptor descriptor,
    String data,
  ) {
    using((Arena arena) {
      final nativeDescriptor = descriptor.toNative(allocator: arena);
      final nativeData = data.toNativeUtf16(allocator: arena);
      final status = _EventWriteWithString(
        regHandle,
        nativeDescriptor,
        nativeData,
      );
      if (status != ERROR_SUCCESS) {
        throw WindowsException(HRESULT_FROM_WIN32(status));
      }
    });
  }

  // ignore: non_constant_identifier_names
  late final _EventRegister = _nativeApi.lookupFunction<
      Int32 Function(
        Pointer<GUID>,
        Pointer<IntPtr>,
        Pointer<IntPtr>,
        Pointer<Uint64>,
      ),
      int Function(
        Pointer<GUID>,
        Pointer<IntPtr>,
        Pointer<IntPtr>,
        Pointer<Uint64>,
      )>('Win32EventRegister');

  // definition for later use
  // ignore: non_constant_identifier_names, unused_field
  late final _EventWrite = _nativeApi.lookupFunction<
      Int32 Function(
        Uint64,
        Pointer<EVENT_DESCRIPTOR>,
        Uint32,
        Pointer<IntPtr>,
      ),
      int Function(
        int,
        Pointer<EVENT_DESCRIPTOR>,
        int,
        Pointer<IntPtr>,
      )>('Win32EventWrite');

  // ignore: non_constant_identifier_names
  late final _EventWriteNoData = _nativeApi.lookupFunction<
      Int32 Function(
        Uint64,
        Pointer<EVENT_DESCRIPTOR>,
      ),
      int Function(
        int,
        Pointer<EVENT_DESCRIPTOR>,
      )>('Win32EventWriteNoData');

  late final _EventWriteWithInt64 = _nativeApi.lookupFunction<
      Int32 Function(
        Uint64,
        Pointer<EVENT_DESCRIPTOR>,
        Int64,
      ),
      int Function(
        int,
        Pointer<EVENT_DESCRIPTOR>,
        int,
      )>('Win32EventWriteWithInt64');

  // ignore: non_constant_identifier_names
  late final _EventWriteWithString = _nativeApi.lookupFunction<
      Int32 Function(
        Uint64,
        Pointer<EVENT_DESCRIPTOR>,
        Pointer<Utf16>,
      ),
      int Function(
        int,
        Pointer<EVENT_DESCRIPTOR>,
        Pointer<Utf16>,
      )>('Win32EventWriteWithString');
}

/// Data class representing an event record written to the event log using ETW for a manifest-based provider
/// See: https://learn.microsoft.com/en-us/windows/win32/api/evntprov/ns-evntprov-event_descriptor
// ignore: camel_case_types
final class EVENT_DESCRIPTOR extends Struct {
  @Uint16()
  // ignore: non_constant_identifier_names
  external int Id;

  @Uint8()
  // ignore: non_constant_identifier_names
  external int Version;

  @Uint8()
  // ignore: non_constant_identifier_names
  external int Channel;

  @Uint8()
  // ignore: non_constant_identifier_names
  external int Level;

  @Uint8()
  // ignore: non_constant_identifier_names
  external int Opcode;

  @Uint16()
  // ignore: non_constant_identifier_names
  external int Task;

  @Uint64()
  // ignore: non_constant_identifier_names
  external int Keyword;
}

extension EventDescriptorExtensions on EventDescriptor {
  Pointer<EVENT_DESCRIPTOR> toNative({Allocator allocator = malloc}) {
    final nativeDescriptor = allocator<Uint8>(sizeOf<EVENT_DESCRIPTOR>());
    final descriptor = nativeDescriptor.cast<EVENT_DESCRIPTOR>();
    descriptor.ref
      ..Id = id
      ..Version = version
      ..Channel = channel
      ..Level = level
      ..Opcode = opcode
      ..Task = task
      ..Keyword = keyword;
    return descriptor;
  }
}

/// Dynamic Library instance shared across all types
final nativeApi = DynamicLibrary.open('EventLogProvider.dll');
