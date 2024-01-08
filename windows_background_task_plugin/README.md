# windows_background_task_plugin

A FFI based plugin for hosting Windows Universal based Background Tasks hosting a Flutter application.

## Getting Started

This [FFI plugin](https://docs.flutter.dev/development/platform-integration/c-interop) provides the reusable infrastructure to host Windows Universal based Background Tasks in a Flutter application.

## Project structure

* `lib`: Contains the Dart code that defines the API of the plugin, and which
  calls into the native code using `dart:ffi`.

* `windows`:  Contains the build files for building and bundling the native code library with the platform application.

## Building and bundling native code

The `pubspec.yaml` specifies FFI plugins as follows:

```yaml
  plugin:
    platforms:
      some_platform:
        ffiPlugin: true
```

The native build systems that are invoked by FFI (and method channel) plugins are:
* For Windows: CMake.
  * See the documentation in windows/CMakeLists.txt.

## Flutter help

For help getting started with Flutter, view our
[online documentation](https://flutter.dev/docs), which offers tutorials,
samples, guidance on mobile development, and a full API reference.

