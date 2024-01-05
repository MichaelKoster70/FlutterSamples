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
#include <flutter/method_result_functions.h>
#include <iostream>
#include <Windows.h>

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

// Method name: native->flutter, execute a task
static constexpr char kExecuteTaskMethodName[] = "executeTask";

// Method name: flutter->native, notify that the channel is initialized
static constexpr char kNotifyChannelInitializedStateMethod[] = "notifyChannelInitialized";

// ----------------------------------------------------------------------------
// Class implementation
// ----------------------------------------------------------------------------

FlutterMethodChannel::FlutterMethodChannel(flutter::BinaryMessenger* messenger, const std::string& channelName)
   : _channel(messenger, channelName, &flutter::StandardMethodCodec::GetInstance())
{
   // Setup the method call handler
   _channel.SetMethodCallHandler([this](const auto& methodCall, auto result)
      {
         HandleMethodCall(methodCall, std::move(result));
      });
}

void FlutterMethodChannel::SetNotifyChannelInitializedHandler(NotifyChannelInitializedHandler handler)
{
   _notifyChannelInitializedHandler = handler;
}

void FlutterMethodChannel::ExecuteTask(const std::string& taskName, ExecuteTaskResultHandler handler)
{
   auto argument = std::make_unique<flutter::EncodableValue>(taskName);
   auto resultHandler = std::make_unique<flutter::MethodResultFunctions<>>(
      [this, handler](const flutter::EncodableValue* successValue)
      {
         auto methodResult = std::get<bool>(*successValue);
         handler(methodResult);
      },
      nullptr, nullptr);

   _channel.InvokeMethod(kExecuteTaskMethodName, std::move(argument), std::move(resultHandler));
}

void FlutterMethodChannel::HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue>& methodCall, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
{
   const std::string& method = methodCall.method_name();
   if (method.compare(kNotifyChannelInitializedStateMethod) == 0)
   {
      // flutter indicated that the channel is initialized
      if (_notifyChannelInitializedHandler)
      {
         _notifyChannelInitializedHandler();
         result->Success();
      }
      else
      {
         result->Error("handler_not_set", "NotifyChannelInitializedHandler not set");
      }
   }
   else
   {
      result->NotImplemented();
   }
}
