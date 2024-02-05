// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Simple event viewer using Windows Event Log Consumer
// ----------------------------------------------------------------------------

// ignore_for_file: avoid_print

import 'package:flutter/material.dart';

import 'package:windows_event_log/event_log_consumer.dart';
import 'package:windows_event_log/event_log_provider.dart';
import 'package:windows_event_log_example/events.dart';

void main() {
  final provider =
      WindowsEventLogProviderFactory.create(Events.sampleProviderId);

  provider.writeEventWithString(Events.logApplicatonInfo, "Sample Info Event");

  // Just for debugging purposes, print all events
  //
  // final app = WindowsEventLog.application;
  // for (final event in app.allEvents) {
  //   print(event.message);
  //   print('------------');
  //   print(event.xml.toString());
  //   print('============');
  // }

  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final app = WindowsEventLog.application;
  final allEvents = List<EventRecord>.empty(growable: true);
  @override
  void initState() {
    super.initState();

    allEvents.addAll(app.allEvents);
  }

  @override
  Widget build(BuildContext context) {
    const textStyle = TextStyle(fontSize: 10);
    const spacerSmall = SizedBox(height: 2);
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Windows Event Viewer'),
        ),
        body: ListView.builder(
          itemCount: allEvents.length,
          itemBuilder: (context, index) {
            final event = allEvents[index];
            return Column(
              children: <Widget>[
                Text(
                  event.message,
                  style: textStyle,
                  textAlign: TextAlign.left,
                ),
                spacerSmall,
                Text(
                  event.xml.toString(),
                  style: textStyle,
                  textAlign: TextAlign.left,
                ),
                spacerSmall,
                const Divider(
                  color: Colors.black,
                  height: 5,
                  thickness: 5,
                  indent: 20,
                  endIndent: 0,
                ),
              ],
            );
          },
        ),
      ),
    );
  }
}
