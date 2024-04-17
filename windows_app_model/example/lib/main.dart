// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
import 'dart:io';
import 'package:windows_appmodel/windows_appmodel.dart';

// ignore_for_file: avoid_print

void main() {
  final appModel = WindowsAppModel();

  print('isSupported: ${appModel.isSupported()}');
  print('isRunningWithIdentity: ${appModel.isRunningWithIdentity()}');

  exit(0);
}
