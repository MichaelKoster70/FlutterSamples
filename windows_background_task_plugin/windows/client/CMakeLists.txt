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
set(PROJECT_NAME "windows_background_task_client")
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED
  "InitializationApis.cpp"
  "BackgroundTaskWithTimeTriggerApis.cpp"
  "BackgroundTaskHelper.cpp"
  "Globals.cpp"
  "version.rc"
)

# make sure the WinRT hgeaders are generated before building this project
add_dependencies(${PROJECT_NAME} "winrt_header_generator")

# Add preprocessor definitions for the build version.
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# add the generated WinRT headers to the include path
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/include)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set_target_properties(${PROJECT_NAME} PROPERTIES
  PUBLIC_HEADER background_task_client_api.h
  OUTPUT_NAME "windows_background_task_client"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC DART_SHARED_LIB)
target_compile_definitions(${PROJECT_NAME} PUBLIC BACKGROUND_TASK_CLIENT_EXPORTS)