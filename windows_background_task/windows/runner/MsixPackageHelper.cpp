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
#include <iostream>
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Management.Deployment.h"
#include "MsixPackageHelper.h"


// ----------------------------------------------------------------------------
// Names usings
// ----------------------------------------------------------------------------
using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Management::Deployment;

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------

bool MsixPackageHelper::RegisterSparsePackage(std::wstring_view externalLocation, std::wstring_view sparsePkgPath)
{
   bool registration = false;
   try
   {
      // Setup URIs for the package and the external location
      Uri externalUri(externalLocation);
      Uri packageUri(sparsePkgPath);

      PackageManager packageManager;

      //Declare use of an external location
      AddPackageOptions options;
      options.ExternalLocationUri(externalUri);

      auto deploymentResult = packageManager.AddPackageByUriAsync(packageUri, options).get();

      registration = deploymentResult.ExtendedErrorCode() == 0;

      if (!registration)
      {
         std::wcout << L"RegisterSparsePackage failed, error code: " << std::hex << deploymentResult.ExtendedErrorCode() << std::endl;
         std::wcout << L"RegisterSparsePackage failed, error message: " << deploymentResult.ErrorText().c_str() << std::endl;
      }
   }
   catch (const hresult_error& ex)
   {
      std::wcout << L"RegisterSparsePackage failed, error message: " << ex.message().c_str() << std::endl;
   }
   return registration;
}

void MsixPackageHelper::UnregisterSparsePackage(std::wstring_view sparsePkgPath)
{
   PackageManager packageManager;
   packageManager.RemovePackageAsync(sparsePkgPath).get();
}
