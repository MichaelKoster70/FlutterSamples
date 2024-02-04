// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';

/// Bindings for `winevt.h` wrappers.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
final class WindowsEventLogConsumerBindings {
  final DynamicLibrary _nativeApi;

  WindowsEventLogConsumerBindings(this._nativeApi);

  /// Creates a new session to query events where
  /// - [path] specifies the path to Log or channel
  /// - [query] specifies the query string
  /// - [flags] specifies the query flags
  ///
  /// see: https://learn.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtquery
  /// Throws a [WindowsException] if the session cannot be created.
  int evtQuery(int session, String path, String query, int flags) {
    return using((Arena arena) {
      final nativePath = path.toNativeUtf16(allocator: arena);
      final nativeQuery = query.toNativeUtf16(allocator: arena);
      final lastError = arena<Uint32>();

      final result =
          _EvtQuery(session, nativePath, nativeQuery, flags, lastError);

      if (result == 0) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }

      return result;
    });
  }

  /// Gets the next event from the query or subscription results indicated by [resultSet].
  /// Gets [eventSize] events and returns them in [events].
  /// returns [true] if there are more events to get.
  (bool success, int returned) evtNext(
    int resultSet,
    int eventSize,
    Pointer<IntPtr> events,
    int timeout,
  ) {
    return using((Arena arena) {
      final lastError = arena<Uint32>();
      final returned = arena<Uint32>();
      final result =
          _EvtNext(resultSet, eventSize, events, timeout, returned, lastError);
      if ((result == 0) && (lastError.value != ERROR_NO_MORE_ITEMS)) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }

      return (result != 0, returned.value);
    });
  }

  /// Renders the values of an event into a buffer
  /// see: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtrender
  /// Throws a [WindowsException] if the event cannot be rendered.
  (bool success, int bufferUsed, int propertyCount) evtRender(
    int context,
    int fragment,
    int flags,
    int bufferSize,
    Pointer<IntPtr> buffer,
  ) {
    return using((Arena arena) {
      final lastError = arena<Uint32>();
      final nativeBufferUsed = arena<Uint32>();
      final nativePropertyCount = arena<Uint32>();
      final result = _EvtRender(context, fragment, flags, bufferSize, buffer,
          nativeBufferUsed, nativePropertyCount, lastError);
      if (result == 0 && lastError.value != ERROR_INSUFFICIENT_BUFFER) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }

      return (result != 0, nativeBufferUsed.value, nativePropertyCount.value);
    });
  }

  /// Opens the specified publisher's metadata.
  /// see: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtopenpublishermetadata
  /// Throws a [WindowsException] if the publisher metadata cannot be opened.
  /// Returns the handle to the publisher metadata.
  int evtOpenPublisherMetadata(
      int session, String publisherId, String? logFilePath, int locale) {
    return using((Arena arena) {
      final nativePublisherId = publisherId.toNativeUtf16(allocator: arena);
      final nativeLogFilePath = logFilePath != null
          ? logFilePath.toNativeUtf16(allocator: arena)
          : nullptr;
      final lastError = arena<Uint32>();

      final result = _EvtOpenPublisherMetadata(
          session, nativePublisherId, nativeLogFilePath, locale, lastError);
      if (result == 0 && lastError.value != ERROR_FILE_NOT_FOUND) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }

      return result;
    });
  }

  /// Gets the message string from the event record.
  /// see: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtformatmessage
  /// Throws a [WindowsException] if the message cannot be retrieved.
  String evtFormatMessage(int publisherMetadata, int event, int messageId) {
    return using((Arena arena) {
      final lastError = arena<Uint32>();
      int bufferSize = 0;
      final bufferUsed = arena<Uint32>();

      // Get the size of the buffer needed to format the message and allocate the buffer
      var result = _EvtFormatMessage(
          publisherMetadata,
          event,
          messageId,
          0,
          nullptr,
          EvtFormatMessageEvent,
          bufferSize,
          nullptr,
          bufferUsed,
          lastError);
      if (result == 0 && lastError.value != ERROR_INSUFFICIENT_BUFFER) {
        switch (lastError.value) {
          case ERROR_EVT_MESSAGE_LOCALE_NOT_FOUND:
            return 'The event definition could not be found for event id ($messageId).';
          case ERROR_EVT_MESSAGE_NOT_FOUND:
            return 'The message resource is present but the message is not found in.';
          default:
            throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
        }
      }

      // allocate the required buffer, and try again - make sure to allocate 2 bytes per character
      bufferSize = bufferUsed.value;
      final buffer = arena.allocate<Utf16>(bufferSize * 2);
      result = _EvtFormatMessage(
          publisherMetadata,
          event,
          messageId,
          0,
          nullptr,
          EvtFormatMessageEvent,
          bufferSize,
          buffer,
          bufferUsed,
          lastError);
      if (result == 0) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }

      return buffer.toDartString();
    });
  }

  /// Closes an open [object] handle.
  /// see: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtclose
  ///
  /// Throws a [WindowsException] if the object cannot be closed.
  void evtClose(int object) {
    using((Arena arena) {
      final lastError = arena<Uint32>();
      final result = _EvtClose(object, lastError);
      if (result == 0) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }
    });
  }

  (String name, String domainName) lookupAccount(String sid) {
    return using((Arena arena) {
      final nativeSid = sid.toNativeUtf8(allocator: arena);
      final nameSize = arena<Uint32>();
      final domainSize = arena<Uint32>();
      final lastError = arena<Uint32>();

      var result = _Win32LookupAccount(nullptr, nativeSid, nullptr, nameSize,
          nullptr, domainSize, lastError);

      if (result == 0 && lastError.value != ERROR_INSUFFICIENT_BUFFER) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }

      final nativeName = arena.allocate<Utf8>(nameSize.value);
      final nativeDomain = arena.allocate<Utf8>(domainSize.value);

      result = _Win32LookupAccount(nullptr, nativeSid, nativeName, nameSize,
          nativeDomain, domainSize, lastError);
      if (result == 0) {
        throw WindowsException(HRESULT_FROM_WIN32(lastError.value));
      }
      return (nativeName.toDartString(), nativeDomain.toDartString());
    });
  }

  // ignore: non_constant_identifier_names
  late final _EvtQuery = _nativeApi.lookupFunction<
      IntPtr Function(
        IntPtr,
        Pointer<Utf16>,
        Pointer<Utf16>,
        Uint32,
        Pointer<Uint32>,
      ),
      int Function(
        int,
        Pointer<Utf16>,
        Pointer<Utf16>,
        int,
        Pointer<Uint32>,
      )>('Win32EvtQuery');

  // ignore: non_constant_identifier_names
  late final _EvtNext = _nativeApi.lookupFunction<
      BOOL Function(
        IntPtr,
        Uint32,
        Pointer<IntPtr>,
        Uint32,
        Pointer<Uint32>,
        Pointer<Uint32>,
      ),
      int Function(
        int,
        int,
        Pointer<IntPtr>,
        int,
        Pointer<Uint32>,
        Pointer<Uint32>,
      )>('Win32EvtNext');

  // ignore: non_constant_identifier_names
  late final _EvtRender = _nativeApi.lookupFunction<
      IntPtr Function(
        IntPtr,
        IntPtr,
        Uint32,
        Uint32,
        Pointer<IntPtr>,
        Pointer<Uint32>,
        Pointer<Uint32>,
        Pointer<Uint32>,
      ),
      int Function(
        int,
        int,
        int,
        int,
        Pointer<IntPtr>,
        Pointer<Uint32>,
        Pointer<Uint32>,
        Pointer<Uint32>,
      )>('Win32EvtRender');

  // ignore: non_constant_identifier_names
  late final _EvtOpenPublisherMetadata = _nativeApi.lookupFunction<
      IntPtr Function(
        IntPtr,
        Pointer<Utf16>,
        Pointer<Utf16>,
        Uint32,
        Pointer<Uint32>,
      ),
      int Function(
        int,
        Pointer<Utf16>,
        Pointer<Utf16>,
        int,
        Pointer<Uint32>,
      )>('Win32EvtOpenPublisherMetadata');

  // ignore: non_constant_identifier_names
  late final _EvtFormatMessage = _nativeApi.lookupFunction<
      IntPtr Function(IntPtr, IntPtr, Uint32, Uint32, Pointer<IntPtr>, Uint32,
          Uint32, Pointer<Utf16>, Pointer<Uint32>, Pointer<Uint32>),
      int Function(
          int,
          int,
          int,
          int,
          Pointer<IntPtr>,
          int,
          int,
          Pointer<Utf16>,
          Pointer<Uint32>,
          Pointer<Uint32>)>('Win32EvtFormatMessage');

  // ignore: non_constant_identifier_names
  late final _EvtClose = _nativeApi.lookupFunction<
      BOOL Function(IntPtr, Pointer<Uint32>),
      int Function(int, Pointer<Uint32>)>('Win32EvtClose');

  // ignore: non_constant_identifier_names
  late final _Win32LookupAccount = _nativeApi.lookupFunction<
      BOOL Function(
        Pointer<Utf8>,
        Pointer<Utf8>,
        Pointer<Utf8>,
        Pointer<Uint32>,
        Pointer<Utf8>,
        Pointer<Uint32>,
        Pointer<Uint32>,
      ),
      int Function(
          Pointer<Utf8>,
          Pointer<Utf8>,
          Pointer<Utf8>,
          Pointer<Uint32>,
          Pointer<Utf8>,
          Pointer<Uint32>,
          Pointer<Uint32>)>('Win32LookupAccount');
}

/// Dynamic Library instance shared across all types
final nativeApi = DynamicLibrary.open('EventLogConsumer.dll');

// -----------------------------------------------------------------------------
// NTSTATUS constants
// -----------------------------------------------------------------------------

/// The data area passed to a system call is too small.
// ignore: constant_identifier_names
const ERROR_INSUFFICIENT_BUFFER = 122;

/// No more data is available.
// ignore: constant_identifier_names
const ERROR_NO_MORE_ITEMS = 259;

// The event definition could not be found for event id (%1).
// ignore: constant_identifier_names
const ERROR_EVT_MESSAGE_LOCALE_NOT_FOUND = 0x3AB9;

/// the message resource is present but the message is not found in.
// ignore: constant_identifier_names
const ERROR_EVT_MESSAGE_NOT_FOUND = 0x3AB3;

// -----------------------------------------------------------------------------
// Enumeration constants
// -----------------------------------------------------------------------------

// EVT_QUERY_FLAGS
// https://docs.microsoft.com/en-us/windows/win32/api/winevt/ne-winevt-evt_query_flags
// ignore: constant_identifier_names
const EvtQueryChannelPath = 0x1;

// ignore: constant_identifier_names
const EvtQueryFilePath = 0x2;

// ignore: constant_identifier_names
const EvtQueryForwardDirection = 0x100;

// ignore: constant_identifier_names
const EvtQueryReverseDirection = 0x200;

// ignore: constant_identifier_names
const EvtQueryTolerateQueryErrors = 0x1000;

// _EVT_RENDER_FLAGS
// ignore: constant_identifier_names
const EvtRenderEventValues = 0;

// ignore: constant_identifier_names
const EvtRenderEventXml = 1;

// ignore: constant_identifier_names
const EvtRenderBookmark = 2;

// _EVT_FORMAT_MESSAGE_FLAGS
// https://docs.microsoft.com/en-us/windows/win32/api/winevt/ne-winevt-evt_format_message_flags

// ignore: constant_identifier_names
const EvtFormatMessageEvent = 1;
