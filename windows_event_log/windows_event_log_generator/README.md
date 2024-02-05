Implements the matchung code generator for the windows_event_log package to automate the generation of event descriptor.
This enables you to automatically keep the manifest file in sync with the DART code.

## Features

* Generates event descriptor entries read from a manifest based Windows ETW provider.

## Getting started

## Usage

Add the following dev dependency to your pubspec.yaml file

```yaml
dev_dependencies:
  build_runner: ^2.4.0
  windows_event_log_generator:
    path: ../../windows_event_log_generator
```    

Add the @ManifestEvents annotation to a mantching class
```dart
@ManifestEvents(manifestPath: 'windows\\data\\sample.man')
final class Events {
  const Events._();
}
```

Run the generator using build_runner
```
dart run build_runner build
```






