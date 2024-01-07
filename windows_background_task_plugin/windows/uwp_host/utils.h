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
/// Configures the given project to use the command line arguments passed in.
/// </summary>
/// <param name="project">The project to be configured.</param>
/// <param name="entryPoint">The entry point to be used for the project.</param>
void ConfigureDartProject(flutter::DartProject& project, std::string entryPoint = std::string());
