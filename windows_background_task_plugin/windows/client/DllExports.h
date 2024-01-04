// ----------------------------------------------------------------------------
// <copyright company="Michael Koster">
//   Copyright (c) Michael Koster. All rights reserved.
//   Licensed under the MIT License.
// </copyright>
// ----------------------------------------------------------------------------

#pragma once

#ifdef BACKGROUND_TASK_CLIENT_EXPORTS
#define BACKGROUND_TASK_CLIENT_API __declspec(dllexport)
#else
#define BACKGROUND_TASK_CLIENT_API __declspec(dllimport)
#endif