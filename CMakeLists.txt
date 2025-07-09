cmake_minimum_required(VERSION 3.21)

#
# Only set the cxx_standard if it is not set by someone else
#
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example when
# compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  kbase_workspace
  VERSION 0.1.0
  DESCRIPTION "Templated cmake project for cross-platform C++ development"
  LANGUAGES CXX)

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/ClangFormat.cmake)
include(ProjectOptions.cmake)

kbase_setup_options()
kbase_global_options()

include(Dependencies.cmake)
kbase_setup_dependencies()
kbase_local_options()

target_compile_features(kbase_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
add_library(kbase_workspace::kbase_options ALIAS kbase_options)
add_library(kbase_workspace::kbase_warnings ALIAS kbase_warnings)

if(kbase_ENABLE_DEVELOPER_MODE)
  enable_testing()
endif()

add_subdirectory(data_structures)
add_subdirectory(app)
