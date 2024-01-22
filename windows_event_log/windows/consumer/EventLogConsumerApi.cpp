// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
// Wrapper API for Windows Event Log Consumer APIs
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "pch.h"
#include <winevt.h>
#include <sddl.h>
#include "EventLogConsumerApi.h"

// ----------------------------------------------------------------------------
// Linker Directives
// ----------------------------------------------------------------------------
#pragma comment(lib, "wevtapi.lib")

// ----------------------------------------------------------------------------
// Implementation
// ----------------------------------------------------------------------------

_Success_(return != 0)
EVT_HANDLE Win32EvtQuery
(
   _In_opt_ EVT_HANDLE Session,
   _In_opt_z_ LPCWSTR Path,
   _In_opt_z_ LPCWSTR Query,
   _In_ DWORD Flags,
   _Out_ PDWORD LastError
)
{
   auto handle = EvtQuery(Session, Path, Query, Flags);
   *LastError = (handle == NULL) ? GetLastError() : ERROR_SUCCESS;
   return handle;
}

_Success_(return != 0)
BOOL Win32EvtNext
(
   _In_ EVT_HANDLE ResultSet,
   DWORD EventsSize,
   _Out_writes_to_(EventsSize, *Returned) PEVT_HANDLE Events,
   DWORD Timeout,
   _Out_range_(0, EventsSize) PDWORD Returned,
   _Out_ PDWORD LastError
)
{
   auto success = EvtNext(ResultSet, EventsSize, Events, Timeout, 0, Returned);
   *LastError = (success == FALSE) ? GetLastError() : ERROR_SUCCESS;
   return success;
}

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
)
{
   auto success = EvtRender(Context, Fragment, Flags, BufferSize, Buffer, BufferUsed, PropertyCount);
   *LastError = (success == FALSE) ? GetLastError() : ERROR_SUCCESS;
   return success;
}

EVT_HANDLE Win32EvtOpenPublisherMetadata
(
   _In_opt_ EVT_HANDLE Session,
   _In_z_ LPCWSTR PublisherId,
   _In_opt_z_ LPCWSTR LogFilePath,
   LCID Locale,
   _Out_ PDWORD LastError
)
{
   auto handle = EvtOpenPublisherMetadata(Session, PublisherId, LogFilePath, Locale, 0);
   *LastError = (handle == NULL) ? GetLastError() : ERROR_SUCCESS;
   return handle;
}

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
)
{
   auto success = EvtFormatMessage(PublisherMetadata, Event, MessageId, ValueCount, Values, Flags, BufferSize, Buffer, BufferUsed);
   *LastError = (success == FALSE) ? GetLastError() : ERROR_SUCCESS;
   return success;
}

_Success_(return != 0)
BOOL Win32EvtClose
(
   _In_ _Post_invalid_ EVT_HANDLE Object,
   _Out_ PDWORD LastError
)
{
   auto success = EvtClose(Object);
   *LastError = (success == FALSE) ? GetLastError() : ERROR_SUCCESS;
   return success;
}

_Success_(return != FALSE)
BOOL Win32LookupAccount
(
   _In_opt_ LPCSTR lpSystemName,
   _In_ LPCSTR lpSidString,
   _Out_writes_to_opt_(*cchName, *cchName + 1) LPSTR Name,
   _Inout_  LPDWORD cchName,
   _Out_writes_to_opt_(*cchReferencedDomainName, *cchReferencedDomainName + 1) LPSTR ReferencedDomainName,
   _Inout_ LPDWORD cchReferencedDomainName,
   _Out_ PDWORD LastError
)
{
   PSID pSid{ 0 };
   if (ConvertStringSidToSidA(lpSidString, &pSid))
   {
      SID_NAME_USE sidNameUse{  };
      DWORD nameSize{ *cchName };
      DWORD domainSize{ *cchReferencedDomainName };
      auto success = LookupAccountSidA(lpSystemName, pSid, Name, &nameSize, ReferencedDomainName, &domainSize, &sidNameUse);
      *cchName = nameSize;
      *cchReferencedDomainName = domainSize;
      *LastError = (success == FALSE) ? GetLastError() : ERROR_SUCCESS;

      LocalFree(pSid);
      return success;
   }
   else
   {
      *LastError = GetLastError();
      return FALSE;
   }
}


