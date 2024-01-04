// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
import 'dart:ffi';

/// Dynamic Library instance shared across all types
final pluginNativeApi =
    DynamicLibrary.open('windows_background_task_client.dll');
