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
/// Method channel for communicating with Flutter code running in the engine.
/// </summary>
class FlutterMethodChannel
{
public:
   FlutterMethodChannel(flutter::BinaryMessenger* messenger, const std::string& channelName);
   virtual ~FlutterMethodChannel() = default;

private:
   std::unique_ptr<flutter::MethodChannel<>> _channel;
};