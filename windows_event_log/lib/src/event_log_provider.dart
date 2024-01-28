// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';
import 'package:windows_event_log/src/event_log_provider_bindings.dart';

/// provides a factory for creating Windows Event Log Providers
abstract class WindowsEventLogProviderFactory {
  /// Create a Windows Event Log Provider with the given [providerId].
  /// The [providerId] must be a valid GUID.
  /// See: https://docs.microsoft.com/en-us/windows/win32/api/evntprov/nf-evntprov-eventregister
  ///
  /// Throws an [ArgumentError] if the [providerId] is not a valid GUID.
  /// Throws an [WindowsException] if creating the provider fails.
  static WindowsEventLogProvider create(Guid providerId) {
    final provider = _WindowsEventLogProvider(providerId);
    provider._register();
    return provider;
  }
}

/// Data class representing an event record written to the event log using ETW for a manifest-based provider
/// This class is the DART native EVENT_DESCRIPTOR structure
///
/// See: https://docs.microsoft.com/en-us/windows/win32/api/evntprov/ns-evntprov-event_descriptor
final class EventDescriptor {
  /// A 16-bit number used to identify manifest-based events.
  final int id;

  /// An 8-bit number used to specify the version of a manifest-based event.
  final int version;

  /// An 8-bit number used to enable special event processing.
  final int channel;

  // An 8-bit number used to describe an event's severity or importance.
  final int level;

  /// An 8-bit number used to mark events with special semantics.
  final int opcode;

  /// A 16-bit number used to annotate an event or related group of events.
  final int task;

  /// A 64-bit bitmask used to indicate an event's membership in a set of event categories.
  final int keyword;

  const EventDescriptor(
    this.id,
    this.version,
    this.channel,
    this.level,
    this.opcode,
    this.task,
    this.keyword,
  );
}

/// Interface defining a simple Windows Event Log Provider
abstract interface class WindowsEventLogProvider {
  /// The provider GUID
  Guid get providerId;

  /// Write an event to the event log identified by the [eventDescriptor] with no additional data.
  void writeEventNoData(EventDescriptor eventDescriptor);

  /// Write an event to the event log identified by the [eventDescriptor] with a single integer as [userData].
  /// The event can have one insertion string (%1).
  void writeEventWithInt64(EventDescriptor eventDescriptor, int userData);

  /// Write an event to the event log identified by the [eventDescriptor] with a single string as [userData].
  /// The event can have one insertion string (%1).
  void writeEventWithString(EventDescriptor eventDescriptor, String userData);
}

class _WindowsEventLogProvider implements WindowsEventLogProvider {
  final Guid _providerId;
  final _bindings = WindowsEventLogProviderBindings(nativeApi);
  late final int _regHandle;

  _WindowsEventLogProvider(this._providerId);

  @override
  Guid get providerId => _providerId;

  @override
  void writeEventNoData(EventDescriptor eventDescriptor) {
    _bindings.eventWriteNoData(_regHandle, eventDescriptor);
  }

  @override
  void writeEventWithInt64(EventDescriptor eventDescriptor, int userData) {
    _bindings.eventWriteWithInt64(_regHandle, eventDescriptor, userData);
  }

  @override
  void writeEventWithString(EventDescriptor eventDescriptor, String userData) {
    _bindings.eventWriteWithString(_regHandle, eventDescriptor, userData);
  }

  void _register() {
    int regHandle;
    (regHandle, _) = _bindings.eventRegister(_providerId);
    _regHandle = regHandle;
  }
}
