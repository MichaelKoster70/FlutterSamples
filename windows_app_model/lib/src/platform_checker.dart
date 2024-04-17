// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
import 'dart:io';

/// Throws an [UnsupportedError] if the current platform is not Windows.
void throwIfNotWindows() {
  if (!Platform.isWindows) {
    throw UnsupportedError('This library is only supported under Windows.');
  }
}
