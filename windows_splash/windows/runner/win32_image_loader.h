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
#include <wincodec.h>
#include <comdef.h>

// -----------------------------------------------------------------------------
// Typedefs
// -----------------------------------------------------------------------------
using IStreamPtr = _com_ptr_t<_com_IIID<IStream, &__uuidof(IStream)>>;
using IWICBitmapSourcePtr =  _com_ptr_t<_com_IIID<IWICBitmapSource, &__uuidof(IWICBitmapSource)>>;

/// <summary>
/// Utility class to load images from win32 app resources.
/// </summary>
class ImageLoader
{
public:
   ImageLoader() = default;
   ~ImageLoader() = default;

   /// <summary>
   /// Load a PNG image from the app's resources.
   /// </summary>
   /// <param name="lpName">The name or ID of the resource.</param>
   /// <returns>The bitmap handle, nullptr in case of failure.</returns>
   HBITMAP Load(LPCTSTR lpName);

private:
   /// <summary>
   /// Load a resource from the app's resources
   /// </summary>
   /// <param name="lpName">The name or ID of the resource.</param>
   /// <param name="lpType">The resource type.</param>
   /// <returns>The stream holding the resource, nullptr in case of failure.</returns>
   IStreamPtr LoadStream(LPCTSTR lpName, LPCTSTR lpType) const;

   /// <summary>
   /// Decode the PNG image stream into a bitmap source.
   /// </summary>
   /// <param name="imageStream">The stream to load the image from</param>
   /// <returns>COM smart pointer holding the image.</returns>
   IWICBitmapSourcePtr LoadPngBitmap(IStreamPtr imageStream) const;

   /// <summary>
   /// Create a bitmap from a bitmap source.
   /// </summary>
   /// <param name="bitmapSource">The source holding the Bitmap</param>
   /// <returns>The bitmap handle.</returns>
   HBITMAP CreateBitmap(IWICBitmapSourcePtr bitmapSource);
};
