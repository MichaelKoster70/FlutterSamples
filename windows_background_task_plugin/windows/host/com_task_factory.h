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

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------

/// <summary>
/// COM class factory for creating the background task instances.
/// </summary>
template <typename F>
struct TaskFactory : implements<TaskFactory<F>, IClassFactory>
{
   HRESULT __stdcall CreateInstance(_In_opt_ IUnknown* aggregateInterface, _In_ REFIID interfaceId, _Outptr_ VOID** object) noexcept final
   {
        if (aggregateInterface != NULL)
        {
             return CLASS_E_NOAGGREGATION;
        }
    
        return make<F>().as(interfaceId, object);
   }

   HRESULT __stdcall LockServer(BOOL lock) noexcept final
   {
   UNREFERENCED_PARAMETER(lock);
   return S_OK;
   }
};
