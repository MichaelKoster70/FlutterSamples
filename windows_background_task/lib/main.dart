// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:windows_background_task_plugin/windows_background_task_client.dart';
import 'package:windows_background_task_plugin/windows_background_task_host.dart';

@pragma('vm:entry-point')
void backgroundCallback() {
  stdout.writeln('DART:: Hello, backgroundCallback()!');
  WindowsBackgroundTaskHost().executeTask((taskName) async {
    stdout.writeln('DART:: Hello, executeTask($taskName)!');
    await Future.delayed(const Duration(seconds: 10));
    return true;
  });
  stdout.writeln('DART:: Goodby, backgroundCallback()!');
}

void main() async {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Windows Background Tasks',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'Windows Background Tasks'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});
  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final _client = WindowsBackgroundTaskClient();
  String _statusText = '';
  _MyHomePageState() {
    _client.initialize("windows_background_task_example");
  }

  void _unregisterAllTasks() {
    try {
      _client.unregisterAllBackgroundTasks();
      setState(() {
        _statusText = 'All tasks unregistered';
      });
    } on WindowsUniversalException catch (e) {
      setState(() {
        _statusText = e.message;
      });
    }
  }

  void _registerComBasedTask() {
    try {
      _client.registerComBackgroundTaskTimer(
          'DartBackgroundTaskTimer', 15, false);
      setState(() {
        _statusText = 'COM Based Task registered';
      });
    } on WindowsUniversalException catch (e) {
      setState(() {
        _statusText = e.message;
      });
    }
  }

  void _unregisterComBasedTask() {
    try {
      _client.unregisterBackgroundTask('DartBackgroundTaskTimer');
      setState(() {
        _statusText = 'COM Based Task unregistered';
      });
    } on WindowsUniversalException catch (e) {
      setState(() {
        _statusText = e.message;
      });
    }
  }

  void _registerUwpBasedTask() {
    try {
      _client.registerUwpBackgroundTaskTimer(
          'DartUwpBackgroundTask', 15, false);
      setState(() {
        _statusText = 'UWP Component Based Task registered';
      });
    } on WindowsUniversalException catch (e) {
      setState(() {
        _statusText = e.message;
      });
    }
  }

  void _unregisteruwpBasedTask() {
    try {
      _client.unregisterBackgroundTask('DartUwpBackgroundTask');
      setState(() {
        _statusText = 'COM Based Task unregistered';
      });
    } on WindowsUniversalException catch (e) {
      setState(() {
        _statusText = e.message;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: ListView(
        children: <Widget>[
          SizedBox(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                RawMaterialButton(
                    onPressed: _unregisterAllTasks,
                    child: const Text('Unregister All Tasks'))
              ],
            ),
          ),
          SizedBox(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                RawMaterialButton(
                  onPressed: _registerComBasedTask,
                  child: const Text('Register COM Based Task'),
                ),
                const SizedBox(width: 10),
                RawMaterialButton(
                  onPressed: _unregisterComBasedTask,
                  child: const Text('Unregister COM Based Task'),
                ),
              ],
            ),
          ),
          SizedBox(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                RawMaterialButton(
                  onPressed: _registerUwpBasedTask,
                  child: const Text('Register UWP Component Based Task'),
                ),
                const SizedBox(width: 10),
                RawMaterialButton(
                  onPressed: _unregisteruwpBasedTask,
                  child: const Text('Unregister UWP Component Based Task'),
                ),
              ],
            ),
          ),
          const SizedBox(height: 10),
          Text('Status: $_statusText')
        ],
      ),
    );
  }
}
