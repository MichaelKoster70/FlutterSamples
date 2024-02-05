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

/// Static class to hold the event descriptors generated from the manifest
@ManifestEvents(manifestPath: 'windows\\data\\sample.man')
final class Events {
  const Events._();

  /// The GUID of the ETW provider
  static Guid get sampleProviderId => _Events.sampleProviderId;

  /// Event descriptor for the logApplicationCritical event writing to the application log
  static EventDescriptor get logApplicationCritical =>
      _Events.logApplicationCritical;

  /// Event descriptor for the logApplicationError event writing to the application log
  static EventDescriptor get logApplicationError => _Events.logApplicationError;

  /// Event descriptor for the logApplicationWarning event writing to the application log
  static EventDescriptor get logApplicationWarning =>
      _Events.logApplicationWarning;

  /// Event descriptor for the logApplicatonInfo event  writing to the application log
  static EventDescriptor get logApplicatonInfo => _Events.logApplicatonInfo;

  /// Event descriptor for the logOperationalCritical event writing to the operational log
  static EventDescriptor get logOperationalCritical =>
      _Events.logOperationalCritical;

  /// Event descriptor for the logOperationalError event writing to the operational log
  static EventDescriptor get logOperationalError => _Events.logOperationalError;

  /// Event descriptor for the logOperationalWarning event writing to the operational log
  static EventDescriptor get logOperationalWarning =>
      _Events.logOperationalWarning;

  /// Event descriptor for the logOperationalInfo event writing to the operational log
  static EventDescriptor get logOperationalInfo => _Events.logOperationalInfo;

  /// Event descriptor for the logDebug event writing to the debug log
  static EventDescriptor get logDebug => _Events.logDebug;
}
