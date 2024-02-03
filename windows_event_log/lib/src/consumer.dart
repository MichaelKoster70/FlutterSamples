// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Library implementing a Windows Event Log Consumer
// See: https://learn.microsoft.com/en-us/windows/win32/wes/consuming-events
// ----------------------------------------------------------------------------

import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';
import 'package:windows_event_log/src/consumer_bindings.dart';
import 'package:xml/xml.dart';
import 'package:xml/xpath.dart';

part 'names.dart';

/// The Serverity level of an event
enum SeverityLevel {
  unknown(value: 0),
  critical(value: 1),
  error(value: 2),
  warning(value: 3),
  information(value: 4),
  verbose(value: 5);

  const SeverityLevel({required this.value});

  final int value;
}

/// Data class representing an event record read from the event log
final class EventRecord {
  /// The event message (Event Viewer: General)
  final String message;

  /// The event channel (Event Viewer: Log Name)
  final String channel;

  /// The event provider name (Event Viewer: source)
  final String providerName;

  /// The event identifier (Event Viewer: Event ID)
  final int id;

  /// The event level (Event Viewer: Level)
  final SeverityLevel level;

  /// The event time (Event Viewer: Logged)
  final DateTime time;

  /// The event user
  final String user;

  /// The event xml (Event Viewer: Details XML View)
  final XmlDocument xml;

  EventRecord(this.message, this.channel, this.providerName, this.id,
      this.level, this.time, this.user, this.xml);
}

/// This interface provides a factory for creating a WindowsEventLogConsumer for well-known event logs
/// such as System, Application, Security, etc.
abstract interface class WindowsEventLog {
  /// Gets the Application event log as [WindowsEventLogConsumer] in context of the current user.
  ///
  /// Throws [UnimplementedError] if the current platform is not Windows.
  /// Throws [WindowsException] if the event log cannot be opened.
  static WindowsEventLogConsumer get application =>
      _WindowsEventLogConsumer(_windowsLogsApplication);

  /// Gets the Security event log as [WindowsEventLogConsumer] in context of the current user.
  ///
  /// Throws [UnimplementedError] if the current platform is not Windows.
  /// Throws [WindowsException] if the event log cannot be opened.
  static WindowsEventLogConsumer get security =>
      _WindowsEventLogConsumer(_windowsLogsSecurity);

  /// Gets the Setup event log as [WindowsEventLogConsumer] in context of the current user.
  ///
  /// Throws [UnimplementedError] if the current platform is not Windows.
  /// Throws [WindowsException] if the event log cannot be opened.
  static WindowsEventLogConsumer get setup =>
      _WindowsEventLogConsumer(_windowsLogsSetup);

  /// Gets the System event log as [WindowsEventLogConsumer] in context of the current user.
  ///
  /// Throws [UnimplementedError] if the current platform is not Windows.
  /// Throws [WindowsException] if the event log cannot be opened.
  static WindowsEventLogConsumer get system =>
      _WindowsEventLogConsumer(_windowsLogsSystem);

  /// Gets the event log with the specified [logName] as [WindowsEventLogConsumer] in context of the current user.
  /// Throws [WindowsException] if the event log cannot be opened.
  static WindowsEventLogConsumer get(String logName) =>
      _WindowsEventLogConsumer(logName);
}

/// Interface
abstract interface class WindowsEventLogConsumer {
  /// Gets all events from the event log as [EventRecord].
  Iterable<EventRecord> get allEvents;
}

class _WindowsEventLogConsumer implements WindowsEventLogConsumer {
  final _bindings = WindowsEventLogConsumerBindings(nativeApi);
  _WindowsEventLogConsumer(this.logName);

  /// The log name in form of 'ProviderName/LogName' or 'LogName'
  final String logName;

  @override
  Iterable<EventRecord> get allEvents sync* {
    final event = calloc<IntPtr>();
    try {
      // Setup: Query all events from the local computer
      final resultSet =
          _bindings.evtQuery(0, logName, '*', EvtQueryChannelPath);

      var (moreItems, _) = _bindings.evtNext(resultSet, 1, event, INFINITE);
      while (moreItems) {
        yield _renderEvent(event.value);
        (moreItems, _) = _bindings.evtNext(resultSet, 1, event, INFINITE);
      }
    } finally {
      calloc.free(event);
    }
  }

  EventRecord _renderEvent(int event) {
    // get the raw XML text
    final xmlText = _renderEventXml(event);
    final xmlDoc = XmlDocument.parse(xmlText);

    final publisherId =
        _getXmlAttributeString(xmlDoc, 'Event/System/Provider/@Name');

    return EventRecord(
        _formatMessage(event, publisherId),
        _getXmlNodeAsString(xmlDoc, 'Event/System/Channel'),
        publisherId,
        _getXmlNodeAsInteger(xmlDoc, 'Event/System/EventID'),
        _getXmlNodeAsSeverityLevel(xmlDoc, 'Event/System/Level'),
        _getXmlAttributeAsDateTime(
            xmlDoc, 'Event/System/TimeCreated/@SystemTime'),
        _getXmlAttributeUserNameString(xmlDoc, 'Event/System/Security/@UserID'),
        xmlDoc);
  }

  String _formatMessage(int event, String publisherId) {
    final publisherMetadata =
        _bindings.evtOpenPublisherMetadata(0, publisherId, null, 0);
    try {
      return publisherMetadata != 0
          ? _bindings.evtFormatMessage(publisherMetadata, event, 0)
          : '';
    } finally {
      if (publisherMetadata != 0) {
        _bindings.evtClose(publisherMetadata);
      }
    }
  }

  String _renderEventXml(int event) {
    return using((Arena arena) {
      // Get the size of the buffer needed to render the event as XML and allocate the buffer
      var (success, bufferUsed, _) =
          _bindings.evtRender(0, event, EvtRenderEventXml, 0, nullptr);
      if (!success && bufferUsed > 0) {
        final bufferSize = bufferUsed;
        // allocate the required buffer, and try again
        final buffer = arena.allocate<Utf16>(bufferSize);

        (success, bufferUsed, _) = _bindings.evtRender(
            0, event, EvtRenderEventXml, bufferSize, buffer.cast());
        return buffer.toDartString();
      }
      return '';
    });
  }

  String _getXmlAttributeString(XmlDocument xmlDoc, String xpath) =>
      xmlDoc.xpath(xpath).firstOrNull?.value ?? '';

  DateTime _getXmlAttributeAsDateTime(XmlDocument xmlDoc, String xpath) {
    final nodeText = xmlDoc.xpath(xpath).firstOrNull?.value;
    if (nodeText != null) {
      return DateTime.tryParse(nodeText) ?? DateTime.now().toUtc();
    }
    return DateTime.now().toUtc();
  }

  String _getXmlAttributeUserNameString(XmlDocument xmlDoc, String xpath) {
    final nodeText = xmlDoc.xpath(xpath).firstOrNull?.value;
    if (nodeText != null) {
      final (userName, domainName) = _bindings.lookupAccount(nodeText);
      return '$domainName\\$userName';
    }
    return '';
  }

  String _getXmlNodeAsString(XmlDocument xmlDoc, String xpath) =>
      xmlDoc.xpath(xpath).firstOrNull?.innerText ?? '';

  int _getXmlNodeAsInteger(XmlDocument xmlDoc, String xpath) {
    final nodeText = xmlDoc.xpath(xpath).firstOrNull?.innerText;
    if (nodeText != null) {
      return int.tryParse(nodeText) ?? 0;
    }
    return 0;
  }

  SeverityLevel _getXmlNodeAsSeverityLevel(XmlDocument xmlDoc, String xpath) {
    final nodeText = xmlDoc.xpath(xpath).firstOrNull?.innerText;
    if (nodeText != null) {
      final index = int.tryParse(nodeText) ?? 0;
      return SeverityLevel.values.firstWhere((e) => e.value == index,
          orElse: () => SeverityLevel.unknown);
    }
    return SeverityLevel.unknown;
  }
}
