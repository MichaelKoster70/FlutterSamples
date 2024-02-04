// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'package:path/path.dart' as p;
import 'package:build/build.dart';
import 'package:analyzer/dart/element/element.dart';
import 'package:code_builder/code_builder.dart';
import 'package:dart_style/dart_style.dart';
import 'package:source_gen/source_gen.dart';
import 'package:windows_event_log/event_log_annotations.dart';
import 'package:windows_event_log_generator/src/build_options.dart';
import 'package:windows_event_log_generator/src/message_compiler.dart';

/// Generator respobsible for generating EventDescriptor classes based on a manifest file.
final class EventDescriptorGenerator
    extends GeneratorForAnnotation<ManifestEvents> {
  final BuildOptions _options;
  const EventDescriptorGenerator(this._options);

  @override
  Future<String> generateForAnnotatedElement(
    Element element,
    ConstantReader annotation,
    BuildStep buildStep,
  ) async {
    // validate that the element is a class
    final eventDescriptorElement = element;
    if (element is! ClassElement) {
      throw InvalidGenerationSourceError(
          '`@ManifestEvents` can only be used on classes.',
          element: eventDescriptorElement);
    }

    // Stage 1: Generate C header file, and parse it
    final manifestPath = annotation.read('manifestPath').stringValue;

    final _FileEntry<_GuildParts> provider;
    final List<_FileEntry<_EventDescriptorParts>> eventDescriptors;
    (provider, eventDescriptors) =
        await _generateAndParseManifestFile(manifestPath);

    final DartEmitter emitter = DartEmitter(useNullSafetySyntax: true);

    // The name of the generated class is the name of the annotated class prefixed with _
    final className = '_${eventDescriptorElement.name ?? '_Events'}';

    final Class eventDescriptorClass =
        Class((ClassBuilder classBuilder) => classBuilder
          ..modifier = ClassModifier.final$
          ..name = className
          ..docs.add(
              '/// Holds the provider GUID and the event descriptors parsed from ${p.basename(manifestPath)}.')
          // Add the provider GUID as a static field
          ..fields.add(Field(
            (field) => field
              ..modifier = FieldModifier.final$
              ..name = provider.name
              ..static = true
              ..type = refer('Guid')
              ..assignment = Code(
                  'Guid.fromComponents(${provider.contents.data1}, ${provider.contents.data2}, ${provider.contents.data3}, ${provider.contents.data4})'),
          ))
          // Add the event descriptors as static fields
          ..fields.addAll(eventDescriptors.map((e) => Field(
                (field) => field
                  ..static = true
                  ..modifier = FieldModifier.constant
                  ..name = e.name
                  ..type = refer('EventDescriptor')
                  ..assignment = Code(
                      'EventDescriptor(${e.contents.id}, ${e.contents.version}, ${e.contents.channel}, ${e.contents.level}, ${e.contents.opcode}, ${e.contents.task}, ${e.contents.keyword})'),
              ))));

    // Stage 2: Generate the source code
    final rawWourceCode = StringBuffer();
    rawWourceCode.writeln(_ignoreLinter);
    rawWourceCode.writeln();
    rawWourceCode.writeln(eventDescriptorClass.accept(emitter));

    // return the formatted source code
    return DartFormatter().format(rawWourceCode.toString());
  }

  /// Generates a C header file from the manifest file and parses it.
  /// Returns the provider GUID and the event descriptors.
  Future<(_FileEntry<_GuildParts>, List<_FileEntry<_EventDescriptorParts>>)>
      _generateAndParseManifestFile(String manifestPath) async {
    // Stage 1: Generate C header file, and parse it
    final messageCompiler = MessageCompiler(_options);
    final rawContents = await messageCompiler.compileHeader(manifestPath);

    // Stage 2: filter, only keep lines with 'GUID' and 'EVENT_DESCRIPTOR' definitions
    final filteredContents = rawContents
        .where((line) =>
            line.contains('GUID') || line.contains('EVENT_DESCRIPTOR'))
        .toList();

    // Stage 3: parse the GUID from the first line
    final guidLine = filteredContents.firstWhere(
      (line) => line.contains('EXTERN_C') && line.contains('GUID'),
      orElse: () => throw Exception(
          'The manifest file does not contain a provider GUID definition.'),
    );

    final provider = _parseGuid(guidLine);

    // Stage 4: parse the EVENT_DESCRIPTOR from the second line
    final eventDescriptors = filteredContents
        .where((line) => line.contains('EVENT_DESCRIPTOR'))
        .map((e) => _parseEventDescriptor(e))
        .toList();

    return (provider, eventDescriptors);
  }

  /// Parses the GUID and the associated symbol name from the given [sourceLine].
  _FileEntry<_GuildParts> _parseGuid(String sourceLine) {
    // split the GUID line into parts, separate by spaces, remove all '{', '}', ',', and ';' characters
    final guidLineParts = sourceLine
        .split(' ')
        .map((element) => element.replaceAll(_removeBracket, ''))
        .toList();

    // the name of the provider is the 5th part of the line
    final providerName = guidLineParts[4];

    // the GUID starts on the 6th part of the line
    final guidData1 = int.parse(guidLineParts[6].substring(2), radix: 16);
    final guidData2 = int.parse(guidLineParts[7].substring(2), radix: 16);
    final guidData3 = int.parse(guidLineParts[8].substring(2), radix: 16);

    // the last part needed to be reversed to form the correct GUID
    final guidData4 = guidLineParts.skip(9).toList().reversed.fold('0x',
        (previousValue, element) => '$previousValue${element.substring(2)}');

    return _FileEntry(
        providerName, _GuildParts(guidData1, guidData2, guidData3, guidData4));
  }

  /// Parses the EVENT_DESCRIPTOR and the associated symbol name from the given [sourceLine].
  _FileEntry<_EventDescriptorParts> _parseEventDescriptor(String sourceLine) {
    // split the lineinto parts, separate by spaces, remove all '{', '}', ',', and ';' characters
    final eventDescriptorLineParts = sourceLine
        .split(' ')
        .map((element) => element.replaceAll(_removeBracket, ''))
        .toList();

    // the name of the event is the 5th part of the line
    final eventName = eventDescriptorLineParts[4];

    // the EVENT_DESCRIPTOR starts on the 6th part of the line
    final eventDescriptor = _EventDescriptorParts(
      eventDescriptorLineParts[6],
      eventDescriptorLineParts[7],
      eventDescriptorLineParts[8],
      eventDescriptorLineParts[9],
      eventDescriptorLineParts[10],
      eventDescriptorLineParts[11],
      eventDescriptorLineParts[12],
    );

    return _FileEntry(eventName, eventDescriptor);
  }
}

final class _GuildParts {
  late final String data1;
  late final String data2;
  late final String data3;
  late final String data4;

  _GuildParts(
      int data1Value, int data2Value, int data3Value, String data4Value) {
    data1 = '0x${data1Value.toRadixString(16)}';
    data2 = '0x${data2Value.toRadixString(16)}';
    data3 = '0x${data3Value.toRadixString(16)}';
    data4 = data4Value;
  }
}

final class _EventDescriptorParts {
  final String id;
  final String version;
  final String channel;
  final String level;
  final String opcode;
  final String task;
  final String keyword;

  const _EventDescriptorParts(this.id, this.version, this.channel, this.level,
      this.opcode, this.task, this.keyword);
}

/// Represents a file entry in the C header file.
final class _FileEntry<T> {
  final String name;
  final T contents;

  const _FileEntry(this.name, this.contents);
}

// constant to ignore linter
const String _ignoreLinter = '// ignore_for_file: type=lint';
final RegExp _removeBracket = RegExp(r'[\{\},;]');
