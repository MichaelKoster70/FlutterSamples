# ----------------------------------------------------------------------------
# <copyright company="Michael Koster">
#   Copyright (c) Michael Koster. All rights reserved.
#   Licensed under the MIT License.
# </copyright>
# ----------------------------------------------------------------------------
# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "windows_event_log")
project(${PROJECT_NAME} LANGUAGES CXX)

# Invoke the build for all native code assets.
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/consumer")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/provider")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/data")
# List of absolute paths to libraries that should be bundled with the plugin.
set(windows_event_log_bundled_libraries
  $<TARGET_FILE:windows_event_log_consumer>
  $<TARGET_FILE:windows_event_log_provider>
  $<TARGET_FILE:windows_event_log_data>
  PARENT_SCOPE
)
