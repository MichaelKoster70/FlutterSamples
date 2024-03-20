// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include <windows.h>
#include "win32_image_loader.h"

#pragma comment(lib, "windowscodecs.lib")

// -----------------------------------------------------------------------------
// Typedefs
// -----------------------------------------------------------------------------
typedef _com_ptr_t<_com_IIID<IWICImagingFactory, &__uuidof(IWICImagingFactory)>> IWICImagingFactoryPtr;
typedef _com_ptr_t<_com_IIID<IWICBitmapDecoder, &__uuidof(IWICBitmapDecoder)>> IWICBitmapDecoderPtr;
typedef _com_ptr_t<_com_IIID<IWICBitmapFrameDecode, &__uuidof(IWICBitmapFrameDecode)>> IWICBitmapFrameDecodePtr;

// ----------------------------------------------------------------------------
// Class implementations
// ----------------------------------------------------------------------------

HBITMAP ImageLoader::Load(LPCTSTR lpName)
{
   HBITMAP hBitmap = nullptr;

   auto imageStream = LoadStream(lpName, L"PNG");
   if (imageStream != nullptr)
   {
      auto bitmapSource = LoadPngBitmap(imageStream);
      if (bitmapSource != nullptr)
      {
         hBitmap = CreateBitmap(bitmapSource);
      }
   }

  return hBitmap;
}

IStreamPtr ImageLoader::LoadStream(LPCTSTR lpName, LPCTSTR lpType)
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
   IStreamPtr stream;
   if (FAILED(CreateStreamOnHGlobal(hBuffer, TRUE, &stream)))
   {
      GlobalFree(hBuffer);
      return nullptr;
   }

   return stream;
}

IWICBitmapSourcePtr ImageLoader::LoadPngBitmap(IStreamPtr imageStream)
{
   try
   {
      _com_ptr_t<IWICBitmapDecoderPtr> pngDecoder(CLSID_WICPngDecoder, nullptr, CLSCTX_INPROC_SERVER);

      if (FAILED(pngDecoder->Initialize(imageStream, WICDecodeMetadataCacheOnLoad)))
      {
         return nullptr;
      }

      // check if there is at least one frame
      UINT nFrameCount = 0;
      if (FAILED(pngDecoder->GetFrameCount(&nFrameCount)) || nFrameCount == 0)
      {
         return nullptr;
      }

      // then load the first frame
      IWICBitmapFrameDecodePtr frame;
      if (FAILED(pngDecoder->GetFrame(0, &frame)))
      {
         return nullptr;
      }

      IWICBitmapSourcePtr bitmapSource;
      if (SUCCEEDED(WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, frame, &bitmapSource)))
      {
         return bitmapSource;
      }
      return nullptr;
   }
   catch (const _com_error&)
   {
      return nullptr;
   }
}

HBITMAP ImageLoader::CreateBitmap(IWICBitmapSourcePtr bitmapSource)
{
   HBITMAP hBitmap = nullptr;

   UINT width = 0;
   UINT height = 0;
   if (FAILED(bitmapSource->GetSize(&width, &height)) || width == 0 || height == 0)
   {
      return nullptr;
   }

   // create a bitmap
   BITMAPINFO bitmapInfo = { 0 };
   bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitmapInfo.bmiHeader.biWidth = width;
   bitmapInfo.bmiHeader.biHeight = -static_cast<LONG>(height);
   bitmapInfo.bmiHeader.biPlanes = 1;
   bitmapInfo.bmiHeader.biBitCount = 32;
   bitmapInfo.bmiHeader.biCompression = BI_RGB;

   void* pvImageBits = nullptr;
   HDC hdcScreen = GetDC(nullptr);
   hBitmap = CreateDIBSection(hdcScreen, &bitmapInfo, DIB_RGB_COLORS, &pvImageBits, nullptr, 0);
   ReleaseDC(NULL, hdcScreen);

   if (hBitmap != nullptr)
   { 
      // extract the image into the HBITMAP
      const UINT cbStride = width * 4;
      const UINT cbImage = cbStride * height;
      if (FAILED(bitmapSource->CopyPixels(nullptr, cbStride, cbImage, static_cast<BYTE*>(pvImageBits))))
      {
         // couldn't extract image; delete HBITMAP
         DeleteObject(hBitmap);
         hBitmap = NULL;
      }
   }
   if (hBitmap == nullptr)
   {
      return nullptr;
   }

   // extract the image into the HBITMAP
   const UINT cbStride = width * 4;
   const UINT cbImage = cbStride * height;
   if (FAILED(bitmapSource->CopyPixels(nullptr, cbStride, cbImage, static_cast<BYTE*>(pvImageBits))))
   {
      // couldn't extract image; delete HBITMAP
      DeleteObject(hBitmap);
      hBitmap = nullptr;
   }

   return hBitmap;
}
