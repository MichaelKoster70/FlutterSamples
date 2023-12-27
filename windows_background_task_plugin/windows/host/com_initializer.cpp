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
#include <winrt/base.h>

// ----------------------------------------------------------------------------
// Implementations
// ----------------------------------------------------------------------------

ComInitializer::ComInitializer()
{
   winrt::init_apartment();
}

ComInitializer::~ComInitializer()
{
   winrt::uninit_apartment();
}