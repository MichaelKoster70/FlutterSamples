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
#include "winrt/Windows.Foundation.h"
#include "com_task_factory.h"

// ----------------------------------------------------------------------------
// Externals
// ----------------------------------------------------------------------------
extern winrt::handle g_processExitEvent;

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------
/// <summary>
/// Utility class responsible for registering the process for COM activation.
/// </summary>
template <typename T>
class ComRegisterProcessForType
{
public:
   ComRegisterProcessForType() = default;
   ~ComRegisterProcessForType()
   {
      // Revoke the class factory registration, ignoring any error.
      if (registrationToken != 0)
      {
         (void)CoRevokeClassObject(registrationToken);
      }
   };

   /// <summary>
   /// Register the given class ID for COM invocation.
   /// </summary>
   /// <returns>HRESULT indicating success or failure.</returns>
   /// <remarks>
   /// cardinality is determined whether the task implementations terminate the main message loop or not.
   /// </remarks>
   hresult Register()
   {
      hresult hr;
      try
      {
         guid classId = __uuidof(T);

         com_ptr<IClassFactory> taskFactory = make<TaskFactory<T>>();
         check_hresult(CoRegisterClassObject(classId, taskFactory.get(), CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &registrationToken));

         // Wait for the signal to terminate the process
         check_hresult(WaitForSingleObject(g_processExitEvent.get(), INFINITE));

         hr = S_OK;
      }
      catch (...)
      {
         hr = to_hresult();
      }

      return hr;
   };

private:
   DWORD registrationToken = 0;
};
