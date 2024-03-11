import 'package:windows_sync/sync_object.dart';

void main() {
  final win32Event = Event.create(manualReset: true, name: 'MyEvent');

  Future.delayed(const Duration(seconds: 2), () {
    win32Event.set();
    win32Event.close();
  });
}
