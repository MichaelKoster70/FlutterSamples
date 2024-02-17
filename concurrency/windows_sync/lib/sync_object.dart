// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:ffi';
import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';

/// Defines an interface for a Windows sync object.
abstract interface class SyncObject {
  /// Returns the handle to the sync object.
  int get handle;

  //// Closes the sync object.
  void close();

  /// Unlocks the sync object.
  /// only used by [SingleLock] to release the lock.
  void _unlock();
}

/// Defines an interface for a Windows event object.
/// See: https://learn.microsoft.com/en-us/windows/win32/sync/event-objects
abstract interface class Event extends SyncObject {
  /// Creates a new unnamed event object with the following properties:
  /// [manualReset] - `true` if the event is a manual-reset event object, `false` if it is an auto-reset event object.
  /// [initialState] is `true`, the event is created in the signaled state.
  /// The [name] parameter is a string that specifies the name of the event object.
  factory Event.create(
          {bool manualReset = false,
          bool initialState = false,
          String? name}) =>
      _Event.create(manualReset, initialState, name);

  factory Event.open(String name,
          {int desiredAccess = SYNCHRONIZE, bool inheritHandle = false}) =>
      _Event.open(name, desiredAccess, inheritHandle);

  /// Sets the event to the signaled state.
  void set();

  /// Resets the event to the non-signaled state.
  void reset();
}

/// Defines an interface for aquiring and releasing an exclusive lock on a Windows sync object.
abstract interface class SingleLock {
  /// Creates a new [SingleLock] object that locks the specified [syncObject].
  factory SingleLock(SyncObject syncObjec) => _SingleLock(syncObjec);

  /// Waits for the lock to be acquired, with a [timeout] in ms.
  /// Returns `true` if the lock was acquired, `false` if the timeout expired.
  ///
  /// The future completes when the lock is acquired or the timeout expires.
  Future<bool> lock(int timeout);

  /// Waits for the lock to be acquired, with a [timeout] in ms.
  /// Returns `true` if the lock was acquired, `false` if the timeout expired.
  ///
  /// This method is synchronous and blocks the current thread inside an OS call.
  bool lockSync(int timeout);

  /// Releases the lock.
  void unlock();
}

class _SingleLock implements SingleLock {
  final SyncObject _syncObject;
  _SingleLock(this._syncObject);

  @override
  bool lockSync(int timeout) {
    final result = WaitForSingleObject(_syncObject.handle, timeout);
    if (result == WAIT_OBJECT_0) {
      return true;
    } else if (result == WAIT_TIMEOUT) {
      return false;
    } else {
      throw WindowsException(HRESULT_FROM_WIN32(GetLastError()));
    }
  }

  @override
  Future<bool> lock(int timeout) {
    return Isolate.run(() async => lockSync(timeout));
  }

  @override
  void unlock() {
    _syncObject._unlock();
  }
}

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
  int get handle => _handle;

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
  void close() {
    // Close the event.
    CloseHandle(_handle);
  }

  @override
  void _unlock() {}
}

// ignore: constant_identifier_names
const int WAIT_OBJECT_0 = 0x00000000;

// ignore: constant_identifier_names
const int WAIT_TIMEOUT = 0x00000102;

// ignore: constant_identifier_names
const int WAIT_FAILED = 0xFFFFFFFF;

// ignore: constant_identifier_names
/// The right to use the object for synchronization. This enables a thread to wait until the object is in the signaled state.
const int SYNCHRONIZE = 0x00100000;
