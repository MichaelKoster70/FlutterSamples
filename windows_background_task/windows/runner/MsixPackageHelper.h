// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Declarations
// ----------------------------------------------------------------------------

/// <summary>
/// Utility class to help with MSIX package operations.
/// </summary>
class MsixPackageHelper
{
public:
   /// <summary>
   /// Registers a sparse MSIX package.
   /// </summary>
   /// <param name="externalLocation">The absolute path of the external package contents.</param>
   /// <param name="sparsePkgPath">The absolute path to the MSIX package to register.</param>
   /// <returns>true if suuccessful, else false</returns>
   static bool RegisterSparsePackage(std::wstring_view externalLocation, std::wstring_view sparsePkgPath);

/// <summary>
/// Unregisters a sparse MSIX package.
/// </summary>
/// <param name="sparsePkgPath">The absolute path to the MSIX package to unregister.</param>
static void UnregisterSparsePackage(std::wstring_view sparsePkgPath);

private:
   MsixPackageHelper() = delete;
   ~MsixPackageHelper() = delete;
   MsixPackageHelper(const MsixPackageHelper&) = delete;
   MsixPackageHelper& operator=(const MsixPackageHelper&) = delete;
   MsixPackageHelper(MsixPackageHelper&&) = delete;
   MsixPackageHelper& operator=(MsixPackageHelper&&) = delete;
};
