# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(sample_plugin_library VERSION 0.0.1 LANGUAGES C)

add_library(sample_plugin SHARED
  "sample_plugin.c"
)

set_target_properties(sample_plugin PROPERTIES
  PUBLIC_HEADER sample_plugin.h
  OUTPUT_NAME "sample_plugin"
)

target_compile_definitions(sample_plugin PUBLIC DART_SHARED_LIB)
