# windows_sync

A Flutter package providing Windows-specific synchronization primitives like Mutex, Semaphore, and Event.

## Getting Started

This project is a starting point for a Flutter
[FFI plugin](https://docs.flutter.dev/development/platform-integration/c-interop),
a specialized package that includes native code directly invoked with Dart FFI.

## Project structure

This package uses the following structure:

* `lib`: Contains the Dart code that defines the API of the plugin, and which
  calls into the native code using `dart:ffi`.

* `windows` : Contains the build files for building and bundling the native code if needed.
