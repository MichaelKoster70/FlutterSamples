// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

#include <string>
#include <flutter/method_channel.h>

/// <summary>
/// Defines the handler for the NotifyChannelInitialized method called by flutter.
/// </summary>
using NotifyChannelInitializedHandler = std::function<void()>;

/// <summary>
/// Defines the handler for the ExecuteTask method results.
/// </summary>
using ExecuteTaskResultHandler = std::function<void(bool)>;

/// <summary>
/// Method channel for communicating with Flutter code running in the engine.
/// </summary>
class FlutterMethodChannel
{
public:
   FlutterMethodChannel(flutter::BinaryMessenger* messenger, const std::string& channelName);
   virtual ~FlutterMethodChannel() = default;

   /// <summary>
   /// Sets the handler for the NotifyChannelInitialized method called by flutter.
   /// </summary>
   /// <param name="handler">The handler</param>
   void SetNotifyChannelInitializedHandler(NotifyChannelInitializedHandler handler);

   /// <summary>
   /// Have the DART VM execute a task with the supplied name.
   /// </summary>
   /// <param name="taskName">The name of the task to execute.</param>
   void ExecuteTask(const std::string& taskName, ExecuteTaskResultHandler handler);

private:
   void HandleMethodCall(const flutter::MethodCall< flutter::EncodableValue>& methodCall, std::unique_ptr< flutter::MethodResult< flutter::EncodableValue>> result);

   flutter::MethodChannel<> _channel;
   NotifyChannelInitializedHandler _notifyChannelInitializedHandler;
};

