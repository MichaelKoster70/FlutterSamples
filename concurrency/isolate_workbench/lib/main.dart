// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Simple console application to illustrate isolates
// ----------------------------------------------------------------------------

// ignore_for_file: avoid_print

import 'dart:isolate';

void main() async {
  DataContainer global = DataContainer();

  Future<void> runInIsolate() {
    final isolateId = identityHashCode(Isolate.current);
    final local = DataContainer();
    print(
        'Isolate N $isolateId: global DataContainer ${identityHashCode(global)}');
    print(
        'Isolate N $isolateId: local DataContainer ${identityHashCode(local)}');
    return Future<void>.value();
  }

  final result1 = Isolate.run(() {
    final isolateId = identityHashCode(Isolate.current);
    final local = DataContainer();
    print(
        'Isolate 1 $isolateId: global DataContainer ${identityHashCode(global)}');
    print(
        'Isolate 1 $isolateId: local DataContainer ${identityHashCode(local)}');
  });

  final result2 = Isolate.run(() {
    final isolateId = identityHashCode(Isolate.current);
    final local = DataContainer();
    print(
        'Isolate 2 $isolateId: global DataContainer ${identityHashCode(global)}');
    print(
        'Isolate 2 $isolateId: local DataContainer ${identityHashCode(local)}');
  });

  final result3 = Isolate.run(runInIsolate);
  final result4 = Isolate.run(runInIsolate);

  print('Main(): global DataContainer ${identityHashCode(global)}');

  await result1;
  await result2;
  await result3;
  await result4;
}

final class DataContainer {
  DataContainer() {
    final isolateId = identityHashCode(Isolate.current);
    print('Isolate $isolateId: DataContainer() ${identityHashCode(this)}');
  }
}
