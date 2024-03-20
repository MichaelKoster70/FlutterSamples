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
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "windows_sync")
project(${PROJECT_NAME} LANGUAGES CXX)

# Define the target name
set(PLUGIN_NAME "windows_sync")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  "Win32SyncApis.h"
  "Win32SyncApis.cpp"
  "DllExports.h"
)

# Define the plugin library target.
add_library(${PLUGIN_NAME} SHARED
  "dllmain.cpp"
  "version.rc"
  ${PLUGIN_SOURCES}
)

# Add preprocessor definitions for the build version.
target_compile_definitions(${PLUGIN_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${PLUGIN_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${PLUGIN_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${PLUGIN_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${PLUGIN_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# Set compiler flags
set_property(TARGET ${PLUGIN_NAME} PROPERTY CXX_STANDARD 17)
target_compile_definitions(${PLUGIN_NAME} PUBLIC DART_SHARED_LIB)
target_compile_definitions(${PLUGIN_NAME} PUBLIC PLUGIN_EXPORTS)
target_compile_options(${PLUGIN_NAME} PRIVATE /W4)
target_compile_options(${PLUGIN_NAME} PRIVATE /WX)

# Update output file name
set_target_properties(${PLUGIN_NAME} PROPERTIES
  OUTPUT_NAME "WindowsSync"
)

set(windows_sync_bundled_libraries
  $<TARGET_FILE:windows_sync>
  PARENT_SCOPE
)