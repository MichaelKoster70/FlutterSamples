# windows_event_log

A new Flutter FFI plugin project.

## Getting Started

This project is a starting point for a Flutter
[FFI plugin](https://docs.flutter.dev/development/platform-integration/c-interop),
a specialized package that includes native code directly invoked with Dart FFI.

## Project structure

This template uses the following structure:

* `src`: Contains the native source code, and a CmakeFile.txt file for building
  that source code into a dynamic library.

* `lib`: Contains the Dart code that defines the API of the plugin, and which
  calls into the native code using `dart:ffi`.

* `windows`: Contains the build files for building and bundling the native code library with the platform application.

## Building and bundling native code

The `pubspec.yaml` specifies FFI plugins as follows:

```yaml
  plugin:
    platforms:
      some_platform:
        ffiPlugin: true
```
# Windows Event Viewer integration
To make the events visible in the event viewer, you need to register the provider. 
This can be done using the wevtutil.exe command line tool. The following command registers the provider: 
```
wevtutil.exe im <path to the manifest file>
```
