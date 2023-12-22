# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)
set(CPPWINRT_VERSION "2.0.230706.1")

# Project-level configuration.
set(PROJECT_NAME "windows_background_task_client")
project(${PROJECT_NAME} LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(nuget
  URL "https://dist.nuget.org/win-x86-commandline/latest/nuget.exe"
  URL_HASH SHA256=cc50b7f3b7e41ef2ea07b56e4e8c36f20357e1534c89b958f9498aa9f24685f40
  DOWNLOAD_NO_EXTRACT true
)

# Have CppWinRT generate the WinRT headers.
find_program(NUGET nuget)
if (NOT NUGET)
    message("Nuget.exe not found, trying to download.")
    FetchContent_MakeAvailable(nuget)
    set(NUGET ${nuget_SOURCE_DIR}/nuget.exe)
endif()

execute_process(COMMAND
    ${NUGET} install Microsoft.Windows.CppWinRT -Version ${CPPWINRT_VERSION} -OutputDirectory packages
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE ret)
if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to install nuget package Microsoft.Windows.CppWinRT.${CPPWINRT_VERSION}")
endif()

set(CPPWINRT ${CMAKE_BINARY_DIR}/packages/Microsoft.Windows.CppWinRT.${CPPWINRT_VERSION}/bin/cppwinrt.exe)
execute_process(COMMAND
    ${CPPWINRT} -input sdk -output include
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE ret)
if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to run cppwinrt.exe")
endif()

# add the generated WinRT headers to the include path
include_directories(BEFORE SYSTEM ${CMAKE_BINARY_DIR}/include)

add_library(${PROJECT_NAME} SHARED
  "InitializationApis.cpp"
  "BackgroundTaskWithTimeTriggerApis.cpp"
  "BackgroundTaskHelper.cpp"
  "Globals.cpp"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set_target_properties(${PROJECT_NAME} PROPERTIES
  PUBLIC_HEADER background_task_client_api.h
  OUTPUT_NAME "windows_background_task_client"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC DART_SHARED_LIB)
target_compile_definitions(${PROJECT_NAME} PUBLIC BACKGROUND_TASK_CLIENT_EXPORTS)