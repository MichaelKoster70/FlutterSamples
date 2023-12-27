// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "flutter_method_channel.h"
#include <flutter/standard_method_codec.h>

FlutterMethodChannel::FlutterMethodChannel(flutter::BinaryMessenger* messenger, const std::string& channelName)
{
   _channel = std::make_unique<flutter::MethodChannel<>>(messenger, channelName, &flutter::StandardMethodCodec::GetInstance());
}
