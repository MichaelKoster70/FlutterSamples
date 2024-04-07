// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "win32_single_instance.h"

Win32SingleInstanceLock::Win32SingleInstanceLock(const std::wstring& defaultMutexName, const std::wstring& windowClasName)
   : defaultMutexName(defaultMutexName), windowClasName(windowClasName)
{
   //EMPTY_BODY
}

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------
Win32SingleInstanceLock::~Win32SingleInstanceLock()
{
   // Close the mutex handle if the mutex was created.
   if (hMutex != nullptr)
   {
      ::CloseHandle(hMutex);
   }
}

bool Win32SingleInstanceLock::IsAnotherInstanceRunning(HINSTANCE hInstance)
{
   // Create a mutex with a name based on the application name.
   std::wstring moduleName = GetMutexNameBasedOnModuleName(hInstance);
   std::wstring mutexName = L"Local\\" + moduleName;

   // Create the mutex initially locked.
   HANDLE mutex = ::CreateMutex(nullptr, TRUE, mutexName.c_str());
   if (mutex == nullptr)
   {
      // The mutex could not be created, so assume another instance is running.
      ActivateExistingInstance();
      return true;
   }

   // Check if the mutex is already owned.
   if (::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      // The mutex is already owned, so another instance is running.
      ::CloseHandle(mutex);
      mutex = nullptr;

      ActivateExistingInstance();
      return true;
   }

   // The mutex is not owned, so this is the first instance.
   return false;
}

std::wstring Win32SingleInstanceLock::GetMutexNameBasedOnModuleName(HINSTANCE hInstance)
{
   auto buffer = GetModuleNameFromInstance(hInstance);
   if (buffer.empty())
   {
      return defaultMutexName;
   }

   // replace backslashes and colons with underscores
   for (wchar_t& c : buffer)
   {
      if (c == L'\\' || c == L':')
      {
         c = L'_';
      }
   }

   return buffer;
}

void Win32SingleInstanceLock::ActivateExistingInstance()
{
   // Activate the existing instance if the window class name is provided.
   if (windowClasName.empty())
   {
      return;
   }

   // Get the our own module path 

   EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
      Win32SingleInstanceLock* classInstance = reinterpret_cast<Win32SingleInstanceLock*>(lParam);

      // Get the window class name
      wchar_t buffer[256];
      if (GetClassName(hWnd, buffer, 256) == 0)
      {
         return TRUE;
      }
      auto className = buffer;

      if (className != classInstance->windowClasName)
      {
         // not the window we are looking for, continue
         return TRUE;
      }

       // at this point we have a window we are looking for

      // Get the module name
      std::wstring otherModuleName = classInstance->GetModuleNameFromWindow(hWnd);
      std::wstring thisModuleName = classInstance->GetModuleNameFromInstance(nullptr);

      if (thisModuleName == otherModuleName)
      {
         // this is our window, activate it
         ShowWindow(hWnd, SW_RESTORE);
         SetForegroundWindow(hWnd);
         return FALSE;
      }

      return TRUE;
      }, reinterpret_cast<LPARAM>(this));
}

std::wstring Win32SingleInstanceLock::GetModuleNameFromInstance(HINSTANCE hInstance)
{
   std::wstring buffer(MAX_PATH, '\0');

   DWORD usedSize = GetModuleFileName(nullptr, buffer.data(), MAX_PATH);
   if (usedSize > 0)
   {
      return buffer;
   }

   // Return an empty string if the module name could not be retrieved.
   return std::wstring();
}

std::wstring Win32SingleInstanceLock::GetModuleNameFromWindow(HWND hWnd)
{
   std::wstring buffer;
   if (DWORD dwProcessId = 0; GetWindowThreadProcessId(hWnd, &dwProcessId) != 0)
   {
      auto processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
      if (processHandle != nullptr)
      {
         buffer = GetModuleNameFromInstance(reinterpret_cast<HINSTANCE>(processHandle));
         CloseHandle(processHandle);
      }
   }

   return buffer;
}
