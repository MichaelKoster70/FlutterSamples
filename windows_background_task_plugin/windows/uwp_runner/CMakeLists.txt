# ----------------------------------------------------------------------------
# <copyright company="Michael Koster">
#   Copyright (c) Michael Koster. All rights reserved.
#   Licensed under the MIT License.
# </copyright>
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "windows_background_task_uwp_runner")
project(${PROJECT_NAME} LANGUAGES CXX)

# Define the application target. To change its name, change BINARY_NAME in the
# top-level CMakeLists.txt, not the value here, or `flutter run` will no longer
# work.
#
# Any new source files that you add to the application should be added here.
add_executable(${PROJECT_NAME} WIN32
  "flutter_engine_host.h"
  "flutter_engine_host.cpp"
  "flutter_method_channel.h"
  "flutter_method_channel.cpp"
  "flutter_window.h"
  "flutter_window.cpp"
  "win32_window.h"
  "win32_window.cpp"
  "main.cpp"
  "utils.h"
  "utils.cpp"
  "com_initializer.h"
  "com_initializer.cpp"
  "debug_log.h"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "uwp_runner.rc"
  "uwp_runner.exe.manifest"
)

# Apply the standard set of build settings. This can be removed for applications that need different build settings.
apply_standard_settings(${PROJECT_NAME})

# Add preprocessor definitions for the build version.
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# Disable Windows macros that collide with C++ standard library functions.
target_compile_definitions(${PROJECT_NAME} PRIVATE "NOMINMAX")

# Set C++ Compiler options
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Set Windows API version to Windows 8.1.
target_compile_definitions(${PROJECT_NAME} PRIVATE "_WIN32_WINNT=0x0603")
target_compile_definitions(${PROJECT_NAME} PRIVATE "WINVER=0x0603")


# Add dependency libraries and include directories. Add any application-specific dependencies here.
target_link_libraries(${PROJECT_NAME} PRIVATE flutter flutter_wrapper_app)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

# Run the Flutter tool portions of the build. This must not be removed.
add_dependencies(${PROJECT_NAME} flutter_assemble)
