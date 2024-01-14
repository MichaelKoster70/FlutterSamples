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


void ConfigureDartProject(flutter::DartProject& project, std::string entryPoint)
{
    project.set_dart_entrypoint(entryPoint);
}

std::string Utf8FromUtf16(const std::wstring& utf16String)
{
   int targetLength = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16String.data(), -1, nullptr, 0, nullptr, nullptr) - 1;
   if (targetLength > 0)
   {
      std::string utf8String;
      utf8String.resize(targetLength);

      int convertedLength = ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16String.c_str(), targetLength, utf8String.data(), targetLength, nullptr, nullptr);
      if (convertedLength != 0)
      {
         return utf8String;
      }
   }

   return std::string();
}

std::vector<std::wstring> GetCommandLineArguments()
{
   int argc;
   wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
   if (argv == nullptr)
   {
      return std::vector<std::wstring>();
   }

   std::vector<std::wstring> commandLineArguments;

   // Skip the first argument as it's the binary name.
   for (int i = 1; i < argc; i++)
   {
      commandLineArguments.push_back(argv[i]);
   }

   ::LocalFree(argv);

   return commandLineArguments;
}
