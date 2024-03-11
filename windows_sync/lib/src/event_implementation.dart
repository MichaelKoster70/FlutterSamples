// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

part of 'event.dart';

class _Event implements Event {
  final int _handle;
  _Event(int handle) : _handle = handle;

  factory _Event.create(bool manualReset, bool initialState, String? name) {
    return using((Arena arena) {
      final handle = CreateEvent(
        nullptr,
        manualReset ? TRUE : FALSE,
        initialState ? TRUE : FALSE,
        name == null ? nullptr : name.toNativeUtf16(allocator: arena),
      );
      if (handle == 0) {
        throw WindowsException(HRESULT_FROM_WIN32(GetLastError()));
      }
      return _Event(handle);
    });
  }

  factory _Event.open(String name, int desiredAccess, bool inheritHandle) {
    return using((Arena arena) {
      final nativeName = name.toNativeUtf16(allocator: arena);
      final handle = OpenEvent(
        desiredAccess,
        inheritHandle ? TRUE : FALSE,
        nativeName,
      );
      if (handle == 0) {
        throw WindowsException(HRESULT_FROM_WIN32(GetLastError()));
      }
      return _Event(handle);
    });
  }

  @override
  void set() {
    // Set the event.
    SetEvent(_handle);
  }

  @override
  void reset() {
    // Reset the event.
    ResetEvent(_handle);
  }

  @override
  bool waitSync(int timeout) {
    final result = WaitForSingleObject(_handle, timeout);
    if (result == WAIT_OBJECT_0) {
      return true;
    } else if (result == WAIT_TIMEOUT) {
      return false;
    } else {
      throw WindowsException(HRESULT_FROM_WIN32(GetLastError()));
    }
  }

  @override
  void close() {
    // Close the event.
    CloseHandle(_handle);
  }
}

class _EventSingleLock implements EventSingleLock {
  final Event _eventObject;
  _EventSingleLock(this._eventObject);

  @override
  bool lockSync(int timeout) => _eventObject.waitSync(timeout);

  @override
  Future<bool> lock(int timeout) {
    return Isolate.run(() async => lockSync(timeout));
  }
}

// ignore: constant_identifier_names
const int WAIT_OBJECT_0 = 0x00000000;

// ignore: constant_identifier_names
const int WAIT_TIMEOUT = 0x00000102;

// ignore: constant_identifier_names
const int WAIT_FAILED = 0xFFFFFFFF;

/// The right to use the object for synchronization. This enables a thread to wait until the object is in the signaled state.
// ignore: constant_identifier_names
const int SYNCHRONIZE = 0x00100000;
