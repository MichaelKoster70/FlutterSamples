// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------
#pragma once

#ifdef EVENT_LOG_PROVIDER_EXPORTS
#define EVENT_LOG_PROVIDER_API __declspec(dllexport)
#else
#define EVENT_LOG_PROVIDER_API __declspec(dllimport)
#endif