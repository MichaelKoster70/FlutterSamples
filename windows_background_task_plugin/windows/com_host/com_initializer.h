// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------
// Class definition
// ----------------------------------------------------------------------------

/// <summary>
/// Utility class to initialize COM in a scope.
/// </summary>
class ComInitializer
{
public:
    ComInitializer();
    ~ComInitializer();
};
