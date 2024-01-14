// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Debug support functions
// ----------------------------------------------------------------------------
#pragma once

// Enable debug logging to the debugger and/or console
// comment out to disable
#if _DEBUG
//#define DEBUG_LOG_DEBUGGER_ENABLED
#define DEBUG_LOG_CONSOLE_ENABLED
#endif

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <string>
#include <format>
#ifdef DEBUG_LOG_DEBUGGER_ENABLED
#include <Windows.h>
#endif
#ifdef DEBUG_LOG_CONSOLE_ENABLED
#include <iostream>
#endif

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------

/// <summary>
/// Writes a debug message to the debugger and console.
/// </summary>
/// <param name="format">The message.</param>
/// <param name="...args">Optional message arguments</param>
/// <remarks>
/// This function is only enabled in debug builds, relies on the C++20 std::format function.
/// </remarks>
template <typename... Args>
void DebugLog(std::string_view fmt, Args&&... args)
{
   std::string formattedMessage = std::vformat(fmt, std::make_format_args(args...));

#ifdef DEBUG_LOG_DEBUGGER_ENABLED
   OutputDebugStringA(formattedMessage.c_str());
#endif

#ifdef DEBUG_LOG_CONSOLE_ENABLED
   std::cout << formattedMessage << std::endl;
#endif
}