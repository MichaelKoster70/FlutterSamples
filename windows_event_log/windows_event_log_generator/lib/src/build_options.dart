// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

/// Represents the options passed to the build runner via build.yaml.
///
/// ```yaml
/// targets:
///   $default:
///     builders:
///       windows_event_log_generator|windows_event_log:
///         enabled: true
///         options:
///           messageCompilerPath: 'C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\mc.exe'
/// ```
final class BuildOptions {
  /// The path to the message compiler tool (mc.exe).
  final String messageCompilerPath;

  const BuildOptions(this.messageCompilerPath);

  factory BuildOptions.fromMap(Map<String, dynamic> map) =>
      BuildOptions(map['messageCompilerPath'] as String);
}
