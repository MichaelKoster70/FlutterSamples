// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:io';
import 'package:path/path.dart' as p;
import 'package:windows_event_log_generator/src/build_options.dart';

/// Provides a class to compile messages from a manifest file calling the message compiler tool (mc.exe).
final class MessageCompiler {
  final BuildOptions _options;

  const MessageCompiler(this._options);

  // Complites the given manifest file specified by [manifestPath] into a C header file and returns its contents.
  // the header file contents is returned as a list of lines.
  Future<List<String>> compileHeader(String manifestPath) async {
    // create a temporary directory for the compiler, and run mc.exe
    final tempDir = _createCompilerTempDir();

    try {
      // get absolute paths for the manifest and header files, and run the compiler
      manifestPath = _getManifestFileAbsolutePath(manifestPath);

      final headerFilePath = _getHeaderFileAbsolutePath(tempDir, manifestPath);
      final result = await Process.run(_options.messageCompilerPath,
          [manifestPath, '-r', tempDir, '-h', tempDir]);

      if (result.exitCode != 0) {
        throw Exception(
            'Failed to compile the manifest file into a header file: ${result.stderr}');
      }

      // read the header file contents and clean up the temporary directory
      final headerFile = File(headerFilePath);
      return await headerFile.readAsLines();
    } finally {
      _cleanupCompilerTempDir(tempDir);
    }
  }

  String _getManifestFileAbsolutePath(String manifestPath) {
    return p.absolute(
      Directory.current.path,
      manifestPath,
    );
  }

  String _getHeaderFileAbsolutePath(String tempDir, String manifestPath) {
    final manifestFile = File(manifestPath);
    final headerFileName = '${p.basenameWithoutExtension(manifestFile.path)}.h';

    return p.join(tempDir, headerFileName);
  }

  String _createCompilerTempDir() => Directory.systemTemp
      .createTempSync('windows_event_log_generator_')
      .absolute
      .path;

  void _cleanupCompilerTempDir(String tempDir) {
    final dir = Directory(tempDir);
    if (dir.existsSync()) {
      dir.deleteSync(recursive: true);
    }
  }
}
