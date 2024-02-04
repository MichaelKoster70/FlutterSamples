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
/// Runs a query to retrieve events from a channel or log file that match the specified query criteria.
/// See: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtquery
/// </summary>
/// <param name="Session">A remote session handle that the EvtOpenSession function returns. Set to NULL to query for events on the local computer..</param>
/// <param name="Path">The name of the channel or the full path to a log file that contains the events that you want to query. .</param>
/// <param name="Query">A query that specifies the types of events that you want to retrieve. </param>
/// <param name="LastError">Reveives the value from GetLastError().</param>
/// <returns>The handle to the open query session.</returns>
EVENT_LOG_CONSUMER_API
_Success_(return != 0)
EVT_HANDLE Win32EvtQuery
(
   _In_opt_ EVT_HANDLE Session,
   _In_opt_z_ LPCWSTR Path,
   _In_opt_z_ LPCWSTR Query,
   _In_ DWORD Flags,
   _Out_ PDWORD LastError
);

// Add XMl documentation as on https://learn.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtnext?devlangs=cpp&f1url=%3FappId%3DDev16IDEF1%26l%3DEN-US%26k%3Dk(WINEVT%252FEvtNext)%3Bk(EvtNext)%3Bk(DevLang-C%252B%252B)%3Bk(TargetOS-Windows)%26rd%3Dtrue
/// <summary>
   /// Retrieves the next block of events from the result set that is returned by the EvtQuery or EvtSubscribe function.
   /// See: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtnext
   /// </summary>
   /// <param name="ResultSet">A handle to the result set that the EvtQuery or EvtSubscribe function returns.</param>
   /// <param name="EventsSize">The maximum number of events to return in the buffer that the Events parameter specifies.</param>
   /// <param name="Events">An array of handles to the events in the result set. The array size is specified by the EventsSize parameter.</param>
   /// <param name="Timeout">The time-out value, in milliseconds, to wait for the next event if the buffer specified by the Events parameter is empty. Use INFINITE to indicate that the function should wait indefinitely for the next event.</param
   /// <param name="Returned">The number of events returned in the buffer that the Events parameter specifies.</param>
   /// <param name="LastError">Reveives the value from GetLastError().</param>
   /// <returns>TRUE if successful; otherwise, FALSE</returns>
   /// 
EVENT_LOG_CONSUMER_API
_Success_(return != 0)
BOOL Win32EvtNext
(
   _In_ EVT_HANDLE ResultSet,
   DWORD EventsSize,
   _Out_writes_to_(EventsSize, *Returned) PEVT_HANDLE Events,
   DWORD Timeout,
   _Out_range_(0, EventsSize) PDWORD Returned,
   _Out_ PDWORD LastError
);

/// <summary>
/// Renders an XML fragment based on the rendering context that you specify.
/// See: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtrender
/// </summary>
/// <param name="Context">A handle to the rendering context that the EvtCreateRenderContext function returns.</param>
/// <param name="Fragment">A handle to the event that you want to render.</param>
/// <param name="Flags">A flag that identifies what to render. </param>
/// <param name="BufferSize">The size of the Buffer buffer, in bytes.</param>
/// <param name="Buffer">A caller-allocated buffer that will receive the rendered output. </param>
/// <param name="BufferUsed">The size, in bytes, of the caller-allocated buffer that the function used or the required buffer size if the function fails with ERROR_INSUFFICIENT_BUFFER.</param>
/// <param name="PropertyCount">The number of the properties in the Buffer parameter if the Flags parameter is set to EvtRenderEventValues; otherwise, zero.</param>
/// <param name="LastError">Reveives the value from GetLastError().</param>
/// <returns>TRUE if successful; otherwise, FALSE</returns>
EVENT_LOG_CONSUMER_API
_Success_(return != 0)
BOOL Win32EvtRender
(
   _In_opt_ EVT_HANDLE Context,
   _In_ EVT_HANDLE Fragment,
   DWORD Flags,
   DWORD BufferSize,
   _Out_writes_bytes_to_opt_(BufferSize, *BufferUsed) PVOID Buffer,
   _Out_ PDWORD BufferUsed,
   _Out_ PDWORD PropertyCount,
   _Out_ PDWORD LastError

);

/// <summary>
/// Gets a handle that you use to read the specified provider's metadata.
/// See: https://docs.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtopenpublishermetadata
/// </summary>
/// <param name="Session">A remote session handle that the EvtOpenSession function returns. Set to NULL to query for events on the local computer.</param>
/// <param name="PublisherId">The name of the provider.</param>
/// <param name="LogFilePath">The full path to the log file that contains the provider's events. Set to NULL when reading the metadata from a registered provider.</param>
/// <param name="Locale">The locale that the provider's message strings are written in.  Set to 0 to use the locale identifier of the calling thread.</param>
/// <param name="LastError">Reveives the value from GetLastError().</param>
/// <returns>TRUE if successful; otherwise, FALSE</returns>
EVENT_LOG_CONSUMER_API
EVT_HANDLE Win32EvtOpenPublisherMetadata
(
   _In_opt_ EVT_HANDLE Session,
   _In_z_ LPCWSTR PublisherId,
   _In_opt_z_ LPCWSTR LogFilePath,
   LCID Locale,
   _Out_ PDWORD LastError
);

/// <summary>
/// Formats a message string.
/// See: https://learn.microsoft.com/en-us/windows/win32/api/winevt/nf-winevt-evtformatmessage
/// </summary>
/// <param name="PublisherMetadata">A handle to the provider's metadata that the EvtOpenPublisherMetadata function returns. </param>
/// <param name="Event">A handle to an event. </param>
/// <param name="MessageId">The resource identifier of the message string that you want to format. </param>
/// <param name="ValueCount">The number of values in the Values parameter.</param>
/// <param name="Values">An array of insertion values to use when formatting the event's message string typically NULL.</param>
/// <param name="Flags">A flag that specifies the message string in the event to format. </param>
/// <param name="BufferSize">The size of the Buffer buffer, in characters..</param>
/// <param name="Buffer">A caller-allocated buffer that will receive the formatted message string.</param>
/// <param name="BufferUsed">The size, in characters of the caller-allocated buffer that the function used or the required buffer size if the function fails with ERROR_INSUFFICIENT_BUFFER.</param>
/// <param name="LastError">Reveives the value from GetLastError().</param>
/// <returns>TRUE if successful; otherwise, FALSE</returns>
EVENT_LOG_CONSUMER_API
_Success_(return != 0)
BOOL Win32EvtFormatMessage
(
   _In_opt_ EVT_HANDLE PublisherMetadata,
   _In_opt_ EVT_HANDLE Event,
   DWORD MessageId,
   DWORD ValueCount,
   _In_reads_opt_(ValueCount) PEVT_VARIANT Values,
   DWORD Flags,
   DWORD BufferSize,
   _Out_writes_to_opt_(BufferSize, *BufferUsed) LPWSTR Buffer,
   _Out_ PDWORD BufferUsed,
_Out_ PDWORD LastError
);

/// <summary>
/// Closes an open handle.
/// </summary>
/// <param name="Object">A handle to an open object.</param>
EVENT_LOG_CONSUMER_API
_Success_(return != 0)
BOOL Win32EvtClose
(
   _In_ _Post_invalid_ EVT_HANDLE Object,
   _Out_ PDWORD LastError
);

EVENT_LOG_CONSUMER_API
_Success_(return != FALSE)
BOOL Win32LookupAccount
(
   _In_opt_ LPCSTR SystemName,
   _In_ LPCSTR SidString,
   _Out_writes_to_opt_(*cchName, *cchName + 1) LPSTR Name,
   _Inout_  LPDWORD cchName,
   _Out_writes_to_opt_(*cchReferencedDomainName, *cchReferencedDomainName + 1) LPSTR ReferencedDomainName,
   _Inout_ LPDWORD cchReferencedDomainName,
   _Out_ PDWORD LastError
);

#ifdef __cplusplus
}
#endif