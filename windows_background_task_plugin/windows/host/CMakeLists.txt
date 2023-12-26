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
set(PROJECT_NAME "windows_background_task_host")
project(${PROJECT_NAME} LANGUAGES CXX)

add_executable(${PROJECT_NAME} WIN32 
  "flutter_engine_host.cpp"
  "flutter_window.cpp"
  "win32_window.cpp"
  "main.cpp"
  "utils.cpp"
  "com_initializer.cpp"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "host.rc"
  "host.manifest"
)

# make sure the WinRT hgeaders are generated before building this project
add_dependencies(${PROJECT_NAME} "winrt_header_generator")

# add the generated WinRT headers to the include path
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/include)

# Apply the standard set of build settings.
apply_standard_settings(${PROJECT_NAME})

# Add preprocessor definitions for the build version.
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# Add dependency libraries and include directories. Add any application-specific dependencies here.
target_link_libraries(${PROJECT_NAME} PRIVATE flutter flutter_wrapper_app)
#target_link_libraries(${PROJECT_NAME} PRIVATE "dwmapi.lib")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

add_dependencies(${PROJECT_NAME} flutter)

# Set the C++ standard to C++ 17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME ${PROJECT_NAME}
)
