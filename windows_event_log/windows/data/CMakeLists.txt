# ----------------------------------------------------------------------------
# <copyright company="Michael Koster">
#   Copyright (c) Michael Koster. All rights reserved.
#   Licensed under the MIT License.
# </copyright>
# ----------------------------------------------------------------------------
# This project holds the data generated from the provider manifest
# See: https://learn.microsoft.com/en-us/windows/win32/wes/using-windows-event-log
# ----------------------------------------------------------------------------
# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "windows_event_log_data")
project(${PROJECT_NAME} LANGUAGES CXX)

# base name for the manifest file (without extension)
set(MANIFEST_FILE "sample")

add_library(${PROJECT_NAME} SHARED 
  "DllMain.cpp"
  "pch.h"
  "version.rc"
  ${CMAKE_CURRENT_BINARY_DIR}/${MANIFEST_FILE}.rc
)

# Add preprocessor definitions for the build version.
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# Add preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE EVENT_LOG_DATA_EXPORTS)

# rule to compile manifest file to resource file
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MANIFEST_FILE}.rc
    COMMAND mc.exe ${CMAKE_CURRENT_SOURCE_DIR}/${MANIFEST_FILE}.man -r ${CMAKE_CURRENT_BINARY_DIR} -h ${CMAKE_CURRENT_BINARY_DIR} 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MANIFEST_FILE}.man
    COMMENT "Compiling ${MANIFEST_FILE}.man to ${MANIFEST_FILE}.rc"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME "EventLogData"
)