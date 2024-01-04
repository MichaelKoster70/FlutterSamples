// ignore_for_file: avoid_print

import 'dart:io';

void main() {
  stdout.writeln('Hello, Windows Console App!');

  stdout.writeln('press any key to exit');
  stdin.readLineSync();
  exit(0);
}
