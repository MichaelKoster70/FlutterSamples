// ignore_for_file: avoid_print

import 'package:windows_sync/sync_object.dart';

void main() async {
  final win32Event = Event.create(manualReset: true, name: 'MyEvent');
  win32Event.reset();

  final lock = EventSingleLock(win32Event);
  final result = await lock.lock(60 * 1000);

  if (result) {
    print('Lock acquired');
  } else {
    print('Lock timed out');
  }
}
