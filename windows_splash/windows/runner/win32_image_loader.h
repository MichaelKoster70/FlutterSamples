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


/// <summary>
/// Utility class to load images from win32 app resources.
/// </summary>
class ImageLoader
{
private:
   /// <summary>
   /// Load a resource from the app's resources
   /// </summary>
   /// <param name="lpName">The name or ID of the resource.</param>
   /// <param name="lpType">The resource type.</param>
   /// <returns>The stream holding the resource, nullptr in case of failure.</returns>
   IStream* Load(LPCTSTR lpName, LPCTSTR lpType);
};

