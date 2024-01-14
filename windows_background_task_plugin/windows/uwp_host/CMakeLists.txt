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

# define Windows SDK and Toolset versions
set(WINDOWS_TARGET_PLATFORM_MIN_VERSION "10.0.17234.0")
set(WINDOWS_TARGET_PLATFORM_VERSION "10.0.22621.0")
set(CPPWINRT_VERSION "2.0.230706.1")
set(WINDOWS_KITS_ROOT_DIR "C:/Program Files (x86)/Windows Kits/10")

set(CMAKE_SYSTEM_NAME WindowsStore)
set(CMAKE_SYSTEM_VERSION 10.0)

cmake_policy(SET CMP0053 NEW)

# Project-level configuration.
set(PROJECT_NAME "windows_background_task_uwp_host")
project(${PROJECT_NAME} LANGUAGES CXX)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Directory.Build.props"
  "${CMAKE_CURRENT_BINARY_DIR}/Directory.Build.props"
  COPYONLY
)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Directory.Build.targets"
  "${CMAKE_CURRENT_BINARY_DIR}/Directory.Build.targets"
  COPYONLY
)

add_library(${PROJECT_NAME} SHARED 
  "dart_background_task.idl"
  "dart_background_task.cpp"
  "dart_background_task.h"
  "debug_log.h"
  "uwp_host.rc"
  "uwp_host.def"
  "${CMAKE_CURRENT_BINARY_DIR}/Generated Files/module.g.cpp"
)

# Model the IDL to implementation file dependency in the generated VS project
set_source_files_properties(dart_background_task.cpp PROPERTIES VS_SETTINGS "DependentUpon=${CMAKE_CURRENT_SOURCE_DIR}/dart_background_task.idl")
set_source_files_properties(dart_background_task.h PROPERTIES VS_SETTINGS "DependentUpon=${CMAKE_CURRENT_SOURCE_DIR}/dart_background_task.idl")

# Add preprocessor definitions for the build version.
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION=\"${FLUTTER_VERSION}\"")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MAJOR=${FLUTTER_VERSION_MAJOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_MINOR=${FLUTTER_VERSION_MINOR}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_PATCH=${FLUTTER_VERSION_PATCH}")
target_compile_definitions(${BINARY_NAME} PRIVATE "FLUTTER_VERSION_BUILD=${FLUTTER_VERSION_BUILD}")

# Set C++ Compiler options
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /wd"4100" /wd"4447")
target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
target_compile_definitions(${PROJECT_NAME} PRIVATE "_HAS_EXCEPTIONS=0")
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")

# set linker options
target_link_options(${PROJECT_NAME} PRIVATE /MANIFEST:NO)

target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:/OPT:NOICF>")
target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:/OPT:NOREF>")

target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/OPT:ICF>")
target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/OPT:REF>")

target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Profile>:/OPT:ICF>")
target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Profile>:/OPT:REF>")

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
  VS_GLOBAL_DefaultLanguage "en-US"
  VS_GLOBAL_ROOTNAMESPACE BackgroundTaskHost
  )

  # Add the C++/WinRT NuGet package to the generated VS project
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY VS_USER_PROPS
  ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.${CPPWINRT_VERSION}/build/native/Microsoft.Windows.CppWinRT.props
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.${CPPWINRT_VERSION}/build/native/Microsoft.Windows.CppWinRT.targets
)

# set_target_properties(${PROJECT_NAME} PROPERTIES 
#   VS_DESKTOP_EXTENSIONS_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}
# )

# Add the WindowsDesktop SDK package to the generated VS project
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY VS_USER_PROPS
  "${WINDOWS_KITS_ROOT_DIR}/Extension SDKs/WindowsDesktop/${WINDOWS_TARGET_PLATFORM_VERSION}/DesignTime/CommonConfiguration/Neutral/WindowsDesktop.props"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_SDK_REFERENCES 
  "WindowsDesktop, Version=${WINDOWS_TARGET_PLATFORM_VERSION}"
)

# override the faulty <Midl> property in the generated VS project
set_property(SOURCE "dart_background_task.idl" PROPERTY VS_SETTINGS "OutputDirectory= ")

set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_WINDOWS_TARGET_PLATFORM_VERSION ${WINDOWS_TARGET_PLATFORM_VERSION}
  VS_WINDOWS_TARGET_PLATFORM_MIN_VERSION ${WINDOWS_TARGET_PLATFORM_MIN_VERSION}
  OUTPUT_NAME "windows_background_task_host"
)