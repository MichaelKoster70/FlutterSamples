// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Wrapper API for ETW based Provider APIs
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "pch.h"
#include <evntprov.h>
#include <sddl.h>
#include "EventLogproviderApi.h"

// ----------------------------------------------------------------------------
// Linker Directives
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Implementation
// ----------------------------------------------------------------------------

ULONG Win32EventRegister
(
   _In_ LPCGUID ProviderId,
   _In_opt_ PENABLECALLBACK EnableCallback,
   _In_opt_ PVOID CallbackContext,
   _Out_ PREGHANDLE RegHandle
)
{
   return EventRegister(ProviderId, EnableCallback, CallbackContext, RegHandle);
}

ULONG Win32EventUnregister
(
   _In_ REGHANDLE RegHandle
)
{
   return EventUnregister(RegHandle);
}

ULONG Win32EventWrite
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor,
   _In_range_(0, MAX_EVENT_DATA_DESCRIPTORS) ULONG UserDataCount,
   _In_reads_opt_(UserDataCount) PEVENT_DATA_DESCRIPTOR UserData
)
{
   return EventWrite(RegHandle, EventDescriptor, UserDataCount, UserData);
}

ULONG Win32EventWriteNoData
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor
)
{
   return EventWrite(RegHandle, EventDescriptor, 0, nullptr);
}

ULONG Win32EventWriteWithInt64
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor,
   _In_ DWORD64 Data
)
{
   EVENT_DATA_DESCRIPTOR eventDataDescriptor;
   EventDataDescCreate(&eventDataDescriptor, &Data, sizeof(Data));
   return EventWrite(RegHandle, EventDescriptor, 1, &eventDataDescriptor);
}

ULONG Win32EventWriteWithString
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor,
   _In_ LPCWSTR Data
)
{
   EVENT_DATA_DESCRIPTOR eventDataDescriptor;
   EventDataDescCreate(&eventDataDescriptor, Data, (ULONG)((wcslen(Data) + 1) * sizeof(WCHAR)));
   return EventWrite(RegHandle, EventDescriptor, 1, &eventDataDescriptor);
}