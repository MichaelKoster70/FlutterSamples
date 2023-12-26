# ----------------------------------------------------------------------------
# <copyright company="Michael Koster">
#   Copyright (c) Michael Koster. All rights reserved.
#   Licensed under the MIT License.
# </copyright>
# ----------------------------------------------------------------------------
# Generates the required Windows Runtime headers for the project.
# ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)
set(CPPWINRT_VERSION "2.0.230706.1")

# Project-level configuration.
set(PROJECT_NAME "winrt_header_generator")
include(FetchContent)

FetchContent_Declare(nuget
  URL "https://dist.nuget.org/win-x86-commandline/v6.8.0/nuget.exe"
  URL_HASH SHA256=6c9e1b09f06971933b08080e7272a2ca5b0d8222500744da757bd8d019013a3d
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

# execute_process(COMMAND
#     ${CPPWINRT} -input sdk -output include
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     RESULT_VARIABLE ret)
# if (NOT ret EQUAL 0)
#     message(FATAL_ERROR "Failed to run cppwinrt.exe")
# endif()

add_custom_target("winrt_header_generator"
    COMMAND ${CPPWINRT} -input sdk -output include
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating WinRT headers"
    VERBATIM)