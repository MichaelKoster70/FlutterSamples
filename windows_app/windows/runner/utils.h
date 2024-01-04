// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// Decralartions
// ----------------------------------------------------------------------------

/// <summary>
/// Creates a console for the process, and redirects stdout and stderr to
// it for both the runner and the Flutter library.
/// </summary>
void CreateAndAttachConsole();

/// <summary>
/// Takes a null-terminated wchar_t* encoded in UTF-16 and returns a std::string encoded in UTF-8
/// </summary>
/// <param name="utf16_string">The source to convert.</param>
/// <returns>The converted string, empty std::string on failure. </returns>
std::string Utf8FromUtf16(const wchar_t* utf16_string);

/// <summary>
/// Gets the command line arguments passed in as a std::vector<std::string>, encoded in UTF-8.
/// </summary>
/// <returns>The command line arguments,an empty std::vector<std::string> on failure. </returns>
std::vector<std::string> GetCommandLineArguments();
