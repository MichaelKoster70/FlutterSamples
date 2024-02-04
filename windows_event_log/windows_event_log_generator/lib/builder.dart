// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

library builder;

import 'package:build/build.dart';
import 'package:source_gen/source_gen.dart';
import 'package:windows_event_log_generator/generator.dart';
import 'package:windows_event_log_generator/src/build_options.dart';

/// Factory method creating the builder triggred by the build system to generate code.
Builder windowsEventLogBuilder(BuilderOptions options) => SharedPartBuilder(
      [EventDescriptorGenerator(BuildOptions.fromMap(options.config))],
      'descriptors',
    );
