// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/// <summary>
/// Registers an ETW event provider, creating a handle that can be used to write ETW events.
/// See: https://learn.microsoft.com/en-us/windows/win32/api/evntprov/nf-evntprov-eventregister
/// </summary>
/// <param name="SourceName">GUID that uniquely identifies the provider, sometimes called a control GUID. This must be a stable identifier so that trace controllers can use the GUID to subscribe to events from this provider.</param>
/// <param name="EnableCallback">Optional EnableCallback that ETW will invoke when a trace session enables or disables this provider. Use NULL if no callback is needed.</param>
///    <param name="CallbackContext">Optional context that ETW will pass to the EnableCallback. Use NULL if no callback is needed.</param>
///    <param name="RegHandle">Handle to the provider that ETW will use to identify the provider in subsequent calls.</param>
///    <returns>ERROR_SUCCESS if successful, otherwise an error code.</returns>
EVENT_LOG_PROVIDER_API
ULONG Win32EventRegister
(
   _In_ LPCGUID ProviderId,
   _In_opt_ PENABLECALLBACK EnableCallback,
   _In_opt_ PVOID CallbackContext,
   _Out_ PREGHANDLE RegHandle
 );

/// <summary>
/// Unregisters an ETW event provider.
/// </summary>
/// <param name="RegHandle">Event provider registration handle returned by EventRegister.</param>
/// <returns>Returns ERROR_SUCCESS if successful.</returns>
EVENT_LOG_PROVIDER_API
ULONG Win32EventUnregister
(
   _In_ REGHANDLE RegHandle
);

/// <summary>
/// Writes an ETW event that uses the current thread's activity ID.
/// </summary>
/// <param name="RegHandle">Registration handle of the provider. The handle comes from EventRegister. The generated event will use the ProviderId associated with the handle.</param>
/// <param name="EventDescriptor">EVENT_DESCRIPTOR with event information (metadata) including ID, Version, Level, Keyword, Channel, Opcode, and Task.</param>
/// <param name="UserDataCount">Number of EVENT_DATA_DESCRIPTOR structures in UserData. The maximum number is 128.</param>
/// <param name="UserData">An array of UserDataCount EVENT_DATA_DESCRIPTOR structures that describe the data to be included in the event. UserData may be NULL if UserDataCount is zero.</param>
/// <returns>Returns ERROR_SUCCESS if successful or an error code.</returns>
EVENT_LOG_PROVIDER_API
ULONG Win32EventWrite
(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor,
    _In_range_(0, MAX_EVENT_DATA_DESCRIPTORS) ULONG UserDataCount,
    _In_reads_opt_(UserDataCount) PEVENT_DATA_DESCRIPTOR UserData
);


/// <summary>
/// Writes an ETW event that uses the current thread's activity ID.
/// </summary>
/// <param name="RegHandle">Registration handle of the provider. The handle comes from EventRegister. The generated event will use the ProviderId associated with the handle.</param>
/// <param name="EventDescriptor">EVENT_DESCRIPTOR with event information (metadata) including ID, Version, Level, Keyword, Channel, Opcode, and Task.</param>
/// <returns>Returns ERROR_SUCCESS if successful or an error code.</returns>
EVENT_LOG_PROVIDER_API
ULONG Win32EventWriteNoData
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor
);

/// <summary>
/// Writes an ETW event that uses the current thread's activity ID.
/// </summary>
/// <param name="RegHandle">Registration handle of the provider. The handle comes from EventRegister. The generated event will use the ProviderId associated with the handle.</param>
/// <param name="EventDescriptor">EVENT_DESCRIPTOR with event information (metadata) including ID, Version, Level, Keyword, Channel, Opcode, and Task.</param>
/// <param name="Data">Data to be included in the event.</param>
/// <returns>Returns ERROR_SUCCESS if successful or an error code.</returns>
EVENT_LOG_PROVIDER_API
ULONG Win32EventWriteWithInt64
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor,
   _In_ DWORD64 Data
);

/// <summary>
/// Writes an ETW event that uses the current thread's activity ID.
/// </summary>
/// <param name="RegHandle">Registration handle of the provider. The handle comes from EventRegister. The generated event will use the ProviderId associated with the handle.</param>
/// <param name="EventDescriptor">EVENT_DESCRIPTOR with event information (metadata) including ID, Version, Level, Keyword, Channel, Opcode, and Task.</param>
/// <param name="Data">Data to be included in the event.</param>
/// <returns>Returns ERROR_SUCCESS if successful or an error code.</returns>
EVENT_LOG_PROVIDER_API
ULONG Win32EventWriteWithString
(
   _In_ REGHANDLE RegHandle,
   _In_ PCEVENT_DESCRIPTOR EventDescriptor,
   _In_ LPCWSTR Data
);

#ifdef __cplusplus
}
#endif