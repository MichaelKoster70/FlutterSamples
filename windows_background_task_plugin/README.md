# windows_background_task_plugin

A Flutter FFI plugin to host Windows Universal based Background Tasks in a Flutter application.

## Getting Started

This [FFI plugin](https://docs.flutter.dev/development/platform-integration/c-interop) provides the reusable infrastructure to host Windows Universal based Background Tasks in a Flutter application.

## Project structure

* `lib`: Contains the Dart code that defines the API of the plugin, and which calls into the native code using `dart:ffi`.
* `windows`:  Contains the build files for building and bundling the native code library with the platform application.
* `windows/client`: Native C++ DLL providing DART friedly API to manage background tasks. This could potententially be written in DART.
* `windows/com_host`: Provide a native C++ EXE implementing an out of process COM Host. This EXE hosts background tasks (IBackgroundTask) with an embedded Flutter VM. The background task directly launches the DART VM to execute the dart script. This requires at least Windows 10 2004.
* `windows/uwp_host`: Provides a native C  ++ DLL implementing the UWP IBackgroundTask interface.Due to UWP API limitations, this DLL cannot host the Flutter VM directly. Instead, it launches a separate win32 console process hosting the VM.
* `windows/uwp_runner`: Provides a modified version of the Flutter runner hosting the DART VM.

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

