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
#include <flutter/dart_project.h>

// ----------------------------------------------------------------------------
// Decralartions
// ----------------------------------------------------------------------------

/// <summary>
/// Creates a console for the process, and redirects stdout and stderr to
// it for both the runner and the Flutter library if needed.
/// </summary>
void CreateAndAttachConsoleIfNeeded();

/// <summary>
/// Creates a console for the process, and redirects stdout and stderr to
// it for both the runner and the Flutter library.
/// </summary>
void CreateAndAttachConsole();

/// <summary>
/// Takes a null-terminated wchar_t* encoded in UTF-16 and returns a std::string encoded in UTF-8.
/// </summary>
/// <param name="utf16_string">The source to convert.</param>
/// <returns>The converted string, empty std::string on failure. </returns>
std::string Utf8FromUtf16(const wchar_t* utf16_string);

/// <summary>
/// Configures the given project to use the command line arguments passed in.
/// </summary>
/// <param name="project">The project to be configured.</param>
/// <param name="entryPoint">The entry point to be used for the project.</param>
void ConfigureDartProject(flutter::DartProject& project, std::string entryPoint = std::string());

/// <summary>
/// Gets the command line arguments passed in as a std::vector<std::string>, encoded in UTF-8.
/// </summary>
/// <returns>The command line arguments,an empty std::vector<std::string> on failure. </returns>
std::vector<std::string> GetCommandLineArguments();
