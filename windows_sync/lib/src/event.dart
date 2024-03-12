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
import 'package:windows_sync/src/event_bindings.dart';

part 'event_implementation.dart';

/// Defines an interface for a Windows event object.
/// See: https://learn.microsoft.com/en-us/windows/win32/sync/event-objects
abstract interface class Event {
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
      _Event.open(desiredAccess, inheritHandle, name);

  /// Sets the event to the signaled state.
  void set();

  /// Resets the event to the non-signaled state.
  void reset();

  /// Waits for the event to be signaled, with a [timeout] in ms.
  /// Returns `true` if the event was signaled, `false` if the timeout expired.
  bool waitSync(int timeout);

  //// Closes the event object.
  void close();
}

/// Defines an interface for aquiring and releasing an exclusive lock on a Windows Event object.
abstract interface class EventSingleLock {
  /// Creates a new [EventSingleLock] object that locks the specified Event [syncObject] .
  factory EventSingleLock(Event syncObject) => _EventSingleLock(syncObject);

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
}
