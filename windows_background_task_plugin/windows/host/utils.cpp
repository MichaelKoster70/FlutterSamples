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
// Defines
// ----------------------------------------------------------------------------
// Uncomment this to always open a console window.
#define ALWAYS_OPEN_CONSOLE

// ----------------------------------------------------------------------------
// Implementations
// ----------------------------------------------------------------------------
void CreateAndAttachConsoleIfNeeded()
{
#if defined(ALWAYS_OPEN_CONSOLE)
   CreateAndAttachConsole();
#else
   // Attach to console when present (e.g., 'flutter run') or create a new console when running with a debugger.
   if (!AttachConsole(ATTACH_PARENT_PROCESS) && ::IsDebuggerPresent())
   {
      CreateAndAttachConsole();
   }
#endif
}

void CreateAndAttachConsole()
{
   if (::AllocConsole())
   {
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
   if (argv == nullptr)
   {
      return std::vector<std::string>();
   }

   std::vector<std::string> commandLineArguments;

   // Skip the first argument as it's the binary name.
   for (int i = 1; i < argc; i++)
   {
      commandLineArguments.push_back(Utf8FromUtf16(argv[i]));
   }

   ::LocalFree(argv);

   return commandLineArguments;
}

std::string Utf8FromUtf16(const wchar_t* utf16String)
{
   if (utf16String == nullptr)
   {
      return std::string();
   }

   int targetLength = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16String, -1, nullptr, 0, nullptr, nullptr) - 1; // remove the trailing null character
   int inputLength = (int)wcslen(utf16String);
   std::string utf8String;
   if (targetLength <= 0 || targetLength > utf8String.max_size())
   {
      return utf8String;
   }

   utf8String.resize(targetLength);
   int converted_length = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16String, inputLength, utf8String.data(), targetLength, nullptr, nullptr);
   if (converted_length == 0)
   {
      return std::string();
   }
   return utf8String;
}
