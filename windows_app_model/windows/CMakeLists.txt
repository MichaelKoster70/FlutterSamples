# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "windows_appmodel")
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED
  "AppModelHelper.h"
  "AppModelHelper.cpp"
  "version.rc"
)

# Add preprocessor definitions for the build version.
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

target_compile_definitions(${PROJECT_NAME} PUBLIC DART_SHARED_LIB)
target_compile_definitions(${PROJECT_NAME} PUBLIC APPMODELHELPER_EXPORTS)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)


set_target_properties(windows_appmodel PROPERTIES
  OUTPUT_NAME "WindowsAppModel"
)

set(windows_appmodel_bundled_libraries
  $<TARGET_FILE:windows_appmodel>
  PARENT_SCOPE
)
