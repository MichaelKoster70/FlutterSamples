// -----------------------------------------------------------------------------
// Copyright (c) Zühlke Engineering AG, Switzerland
// -----------------------------------------------------------------------------
// Exception thrown then a native Win32 API fails unexpectedly
// -----------------------------------------------------------------------------

/// Exception thrown when a UWP API fails unexpectedly.
class WindowsUniversalException implements Exception {
  final String message;
  final int hResult;

  const WindowsUniversalException(this.message, this.hResult);

  @override
  String toString() => '$message - HR=0x${hResult.toRadixString(16)}';
}
