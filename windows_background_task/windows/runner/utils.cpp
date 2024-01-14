// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "utils.h"
#include <flutter_windows.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>

// ----------------------------------------------------------------------------
// Implementations
// ----------------------------------------------------------------------------

void CreateAndAttachConsoleIfNeeded()
{
   // Attach to console when present (e.g., 'flutter run') or create a new console when running with a debugger.
   if (!AttachConsole(ATTACH_PARENT_PROCESS) && ::IsDebuggerPresent())
   {
      CreateAndAttachConsole();
   }
}

void CreateAndAttachConsole()
{
   if (::AllocConsole()) {
      FILE* unused;
      if (freopen_s(&unused, "CONOUT$", "w", stdout))
      {
         (void)_dup2(_fileno(stdout), 1);
      }
      if (freopen_s(&unused, "CONOUT$", "w", stderr))
      {
         (void)_dup2(_fileno(stdout), 2);
      }
      std::ios::sync_with_stdio();
      FlutterDesktopResyncOutputStreams();
   }
}

void ConfigureDartProject(flutter::DartProject& project)
{
   auto commandLineArguments = GetCommandLineArguments();
   project.set_dart_entrypoint_arguments(std::move(commandLineArguments));
}

std::vector<std::string> GetCommandLineArguments()
{
  // Convert the UTF-16 command line arguments to UTF-8 for the Engine to use.
  int argc;
  wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  if (argv == nullptr) {
    return std::vector<std::string>();
  }

  std::vector<std::string> command_line_arguments;

  // Skip the first argument as it's the binary name.
  for (int i = 1; i < argc; i++) {
    command_line_arguments.push_back(Utf8FromUtf16(argv[i]));
  }

  ::LocalFree(argv);

  return command_line_arguments;
}

std::string Utf8FromUtf16(const wchar_t* utf16String) {
   if (utf16String == nullptr)
   {
      return std::string();
   }

   int target_length = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16String, -1, nullptr, 0, nullptr, nullptr) - 1; // remove the trailing null character
   int input_length = (int)wcslen(utf16String);
   std::string utf8_string;
   if (target_length <= 0 || target_length > utf8_string.max_size()) {
      return utf8_string;
   }
   utf8_string.resize(target_length);
   int converted_length = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16String, input_length, utf8_string.data(), target_length, nullptr, nullptr);
   if (converted_length == 0)
   {
      return std::string();
   }
   return utf8_string;
}
