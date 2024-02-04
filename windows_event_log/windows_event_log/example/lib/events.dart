// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Event Descriptor definitions
// ----------------------------------------------------------------------------

import 'package:win32/win32.dart';
import 'package:windows_event_log/event_log_provider.dart';
import 'package:windows_event_log/event_log_annotations.dart';

part 'events.g.dart';

// Provider GUID = 6018d8a9-5ead-44f8-ab34-9c795b3d2c84
// EXTERN_C __declspec(selectany) const GUID sample_provider = {0x6018d8a9, 0x5ead, 0x44f8, {0xab, 0x34, 0x9c, 0x79, 0x5b, 0x3d, 0x2c, 0x84}};
final sampleProviderId =
    Guid.fromComponents(0x6018d8a9, 0x5ead, 0x44f8, 0x842c3d5b799c34ab);

// Event Descriptor definitions
const logApplicationCritical =
    EventDescriptor(0x1, 0x1, 0x9, 0x1, 0x0, 0x0, 0x4000000000000000);
const logApplicationError =
    EventDescriptor(0x2, 0x1, 0x9, 0x2, 0x0, 0x0, 0x4000000000000000);
const logApplicationWarning =
    EventDescriptor(0x3, 0x1, 0x9, 0x3, 0x0, 0x0, 0x4000000000000000);
const logApplicatonInfo =
    EventDescriptor(0x4, 0x1, 0x9, 0x4, 0x0, 0x0, 0x4000000000000000);
const logOperationalCritical =
    EventDescriptor(0x5, 0x1, 0x10, 0x4, 0x0, 0x0, 0x8000000000000000);
const logOperationalError =
    EventDescriptor(0x6, 0x1, 0x10, 0x3, 0x0, 0x0, 0x8000000000000000);
const logOperationalWarning =
    EventDescriptor(0x7, 0x1, 0x10, 0x3, 0x0, 0x0, 0x8000000000000000);
const logOperationalInfo =
    EventDescriptor(0x8, 0x1, 0x10, 0x4, 0x0, 0x0, 0x8000000000000000);
const logDebug =
    EventDescriptor(0x9, 0x1, 0x11, 0x5, 0x0, 0x0, 0x2000000000000000);

@ManifestEvents(manifestPath: 'windows\\data\\sample.man')
final class Events {
  const Events._();

  static Guid get sampleProviderId => _Events.sampleProviderId;

  static EventDescriptor get logApplicationCritical =>
      _Events.logApplicationCritical;
}
