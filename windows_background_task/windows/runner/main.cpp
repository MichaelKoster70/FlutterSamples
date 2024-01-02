// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <flutter/dart_project.h>
#include <flutter/flutter_view_controller.h>
#include <windows.h>
#include <iostream>
#include <filesystem>

#include "flutter_window.h"
#include "utils.h"
#include "ExecutionMode.h"
#include "MsixPackageHelper.h"

// ----------------------------------------------------------------------------
// App entry point
// ----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ wchar_t* pCmdLine, _In_ int nCmdShow)
{
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(pCmdLine);
   UNREFERENCED_PARAMETER(nCmdShow);

   CreateAndAttachConsoleIfNeeded();

   // if we are not running with identity, then we need to register the sparse package
   if (!ExecutionMode::IsRunningWithIdentity())
   {
      auto exePath = std::wstring(MAX_PATH, L'\0');
      std::wstring externalLocation;
      std::wstring sparsePkgPath = L"windows_background_task.msix";
      if (GetModuleFileName(nullptr, exePath.data(), (DWORD)exePath.size()) == 0)
      {
         std::cout << "Failed to get module file name\n";
         return 1;
      }

      {
         std::filesystem::path exePathPath(exePath);
         exePathPath.remove_filename();
         externalLocation = exePathPath.wstring();
         exePathPath.append(sparsePkgPath);
         sparsePkgPath = exePathPath.wstring();
      }

      auto registered = MsixPackageHelper::RegisterSparsePackage(externalLocation, sparsePkgPath);
      if (registered)
      {
         std::cout << "Sparse package registered - relaunching\n";
         STARTUPINFO si = {  };
         PROCESS_INFORMATION pi = {  };
         si.cb = sizeof(si);
         if (CreateProcess(exePath.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
         {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return 0;
         }
      }
      else
      {
         std::cout << "Failed to register sparse package\n";
         return 1;
      }
   }

   // Initialize COM, so that it is available for use in the library and plugins.
   (void)::CoInitializeEx(nullptr, COINIT_MULTITHREADED);

   flutter::DartProject project(L"data");

   ConfigureDartProject(project);

   FlutterWindow window(project);
   Win32Window::Point origin(10, 10);
   Win32Window::Size size(1280, 720);
   if (!window.Create(L"windows_background_task", origin, size))
   {
      return EXIT_FAILURE;
   }
   window.SetQuitOnClose(true);

   ::MSG msg;
   while (::GetMessage(&msg, nullptr, 0, 0))
   {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
   }

   ::CoUninitialize();
   return EXIT_SUCCESS;
}
