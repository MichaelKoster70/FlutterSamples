// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <windows.h>
#include "win32_image_loader.h"

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------

IStream* ImageLoader::Load(LPCTSTR lpName, LPCTSTR lpType)
{
   // find the resource
   HRSRC hResInfo = FindResource(nullptr, lpName, lpType);
   if (hResInfo == nullptr)
   {
      return nullptr;
   }

   // load the image resource
   DWORD imageSize = SizeofResource(nullptr, hResInfo);
   HGLOBAL hImageData = LoadResource(nullptr, hResInfo);
   if (hImageData == nullptr)
   {
      return nullptr;
   }

   // lock the image resource
   LPVOID pImageData = LockResource(hImageData);

   // allocate a buffer for the image
   HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
   if (hBuffer == nullptr)
   {
      return nullptr;
   }

   // copy the image resource to the buffer
   LPVOID pBuffer = GlobalLock(hBuffer);
   if (pBuffer == nullptr)
   {
      GlobalFree(hBuffer);
      return nullptr;
   }

   CopyMemory(pBuffer, pImageData, imageSize);
   GlobalUnlock(hBuffer);

   // create a stream from the image resource
   IStream* pStream = nullptr;
   if (FAILED(CreateStreamOnHGlobal(hBuffer, TRUE, &pStream)))
   {
      GlobalFree(hBuffer);
      return nullptr;
   }

   return pStream;
}
