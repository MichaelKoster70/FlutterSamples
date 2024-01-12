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
#include "com_initializer.h"

// ----------------------------------------------------------------------------
// Implementations
// ----------------------------------------------------------------------------

ComInitializer::ComInitializer(_In_ DWORD dwCoInit)
{
   (void)::CoInitializeEx(nullptr, dwCoInit);
}

ComInitializer::~ComInitializer()
{
   ::CoUninitialize();
}