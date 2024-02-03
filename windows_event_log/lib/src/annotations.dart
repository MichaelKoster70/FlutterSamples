// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

/// Annotation used to to speficy the class to contain the event definitions read from a manifest file.
///
/// See: https://learn.microsoft.com/en-us/windows/win32/etw/writing-manifest-based-events
final class ManifestEvents {
  /// The relative path to the manifest file (.man) holding the event definitions
  final String manifestPath;

  const ManifestEvents({required this.manifestPath});
}
