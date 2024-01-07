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

set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0)

# Project-level configuration.
set(PROJECT_NAME "windows_background_task_uwp_host")
set(CPPWINRT_VERSION "2.0.230706.1")
project(${PROJECT_NAME} LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED 
  "flutter_engine_host.cpp"
  "flutter_window.cpp"
  "flutter_method_channel.cpp"
  "win32_window.cpp"
  "utils.cpp"
  "${FLUTTER_MANAGED_DIR}/generated_plugin_registrant.cc"
  "host.rc"
  "host.manifest"
  "dart_background_task.idl"
  "uwp_host.def"
)

# override the faulty <Midl> property in the generated VS project
set_property(SOURCE "dart_background_task.idl" PROPERTY VS_SETTINGS "OutputDirectory= ")

# make sure the WinRT hgeaders are generated before building this project
add_dependencies(${PROJECT_NAME} "winrt_header_generator")

# add the generated WinRT headers to the include path
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/include)

# Apply the standard set of build settings.
#apply_standard_settings(${PROJECT_NAME})

# Add preprocessor definitions for the build version.
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# Add dependency libraries and include directories. Add any application-specific dependencies here.
target_link_libraries(${PROJECT_NAME} PRIVATE flutter flutter_wrapper_app)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

add_dependencies(${PROJECT_NAME} flutter)

# Set the C++ standard to C++ 17
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Set the C++/WinRT language option in the generated VS project
set_target_properties(${PROJECT_NAME} PROPERTIES 
  VS_GLOBAL_CppWinRTOptimized "true"
  VS_GLOBAL_CppWinRTProjectLanguage "C++/WinRT"
  VS_GLOBAL_CppWinRTRootNamespaceAutoMerge "true"
  VS_GLOBAL_CppWinRTGenerateWindowsMetadata "true"
  VS_GLOBAL_MinimalCoreWin "true"
  VS_GLOBAL_ApplicationType "Windows Store"
  VS_GLOBAL_ApplicationTypeRevision "10.0"
  VS_GLOBAL_AppContainerApplication "true"
  VS_GLOBAL_ROOTNAMESPACE BackgroundTaskHost
  )

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY VS_USER_PROPS
  ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.${CPPWINRT_VERSION}/build/native/Microsoft.Windows.CppWinRT.props
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.${CPPWINRT_VERSION}/build/native/Microsoft.Windows.CppWinRT.targets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_WINDOWS_TARGET_PLATFORM_MIN_VERSION 10.0.17134.0
  OUTPUT_NAME "windows_background_task_host"
)