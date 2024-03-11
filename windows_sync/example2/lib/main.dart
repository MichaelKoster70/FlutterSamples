import 'package:windows_sync/sync_object.dart';

void main() async {
  final win32Event = Event.create(manualReset: true, name: 'MyEvent');

  final lock = EventSingleLock(win32Event);
  await lock.lock(60 * 1000);
}
