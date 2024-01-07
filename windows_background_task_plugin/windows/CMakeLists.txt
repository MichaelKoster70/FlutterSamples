# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "windows_background_task_plugin")
project(${PROJECT_NAME} LANGUAGES CXX)

# Invoke the build for native code shared with the other target platforms.
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/winrt")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/com_host")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/uwp_host")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/client")

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(windows_background_task_plugin_bundled_libraries
$<TARGET_FILE:windows_background_task_client>
$<TARGET_FILE:windows_background_task_com_host>
$<TARGET_FILE:windows_background_task_uwp_host>
 PARENT_SCOPE
)
