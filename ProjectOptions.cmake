include(cmake/SystemLink.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(kbase_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES
                                                   ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES
                                                   ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(kbase_setup_options)
  option(kbase_ENABLE_HARDENING "Enable hardening" OFF)
  option(kbase_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    kbase_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies" ON
    kbase_ENABLE_HARDENING OFF)

  kbase_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR kbase_PACKAGING_MAINTAINER_MODE)
    option(kbase_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(kbase_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(kbase_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(kbase_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(kbase_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(kbase_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(kbase_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(kbase_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(kbase_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(kbase_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(kbase_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(kbase_ENABLE_PCH "Enable precompiled headers" OFF)
    option(kbase_ENABLE_CACHE "Enable ccache" OFF)
    option(kbase_ENABLE_DEVELOPER_MODE "Enable unit tests and formatting" OFF)
  else()
    option(kbase_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(kbase_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(kbase_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(kbase_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer"
           ${SUPPORTS_ASAN})
    option(kbase_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(kbase_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer"
           ${SUPPORTS_UBSAN})
    option(kbase_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(kbase_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(kbase_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(kbase_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(kbase_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(kbase_ENABLE_PCH "Enable precompiled headers" OFF)
    option(kbase_ENABLE_CACHE "Enable ccache" ON)
    option(kbase_ENABLE_DEVELOPER_MODE "Enable unit tests and formatting" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      kbase_ENABLE_IPO
      kbase_WARNINGS_AS_ERRORS
      kbase_ENABLE_USER_LINKER
      kbase_ENABLE_SANITIZER_ADDRESS
      kbase_ENABLE_SANITIZER_LEAK
      kbase_ENABLE_SANITIZER_UNDEFINED
      kbase_ENABLE_SANITIZER_THREAD
      kbase_ENABLE_SANITIZER_MEMORY
      kbase_ENABLE_UNITY_BUILD
      kbase_ENABLE_CLANG_TIDY
      kbase_ENABLE_CPPCHECK
      kbase_ENABLE_COVERAGE
      kbase_ENABLE_PCH
      kbase_ENABLE_CACHE
      kbase_ENABLE_DEVELOPER_MODE)
  endif()
endmacro()

macro(kbase_global_options)
  if(kbase_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    kbase_enable_ipo()
  endif()

  kbase_supports_sanitizers()

  if(kbase_ENABLE_HARDENING AND kbase_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR kbase_ENABLE_SANITIZER_UNDEFINED
       OR kbase_ENABLE_SANITIZER_ADDRESS
       OR kbase_ENABLE_SANITIZER_THREAD
       OR kbase_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message(
      "${kbase_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${kbase_ENABLE_SANITIZER_UNDEFINED}"
    )
    kbase_enable_hardening(kbase_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(kbase_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(kbase_warnings INTERFACE)
  add_library(kbase_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  set_kbase_warnings(kbase_warnings ${kbase_WARNINGS_AS_ERRORS} "" "" ""
                       "")

  if(kbase_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    kbase_configure_linker(kbase_options)
  endif()

  include(cmake/Sanitizers.cmake)
  kbase_enable_sanitizers(
    kbase_options ${kbase_ENABLE_SANITIZER_ADDRESS}
    ${kbase_ENABLE_SANITIZER_LEAK} ${kbase_ENABLE_SANITIZER_UNDEFINED}
    ${kbase_ENABLE_SANITIZER_THREAD} ${kbase_ENABLE_SANITIZER_MEMORY})

  set_target_properties(kbase_options
                        PROPERTIES UNITY_BUILD ${kbase_ENABLE_UNITY_BUILD})

  if(kbase_ENABLE_PCH)
    target_precompile_headers(kbase_options INTERFACE <vector> <string>
                              <utility>)
  endif()

  if(kbase_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    kbase_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(kbase_ENABLE_CLANG_TIDY)
    kbase_enable_clang_tidy(kbase_options ${kbase_WARNINGS_AS_ERRORS})
  endif()

  if(kbase_ENABLE_CPPCHECK)
    kbase_enable_cppcheck(
      ${kbase_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(kbase_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    kbase_enable_coverage(kbase_options)
  endif()

  if(kbase_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(kbase_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(kbase_ENABLE_HARDENING AND NOT kbase_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR kbase_ENABLE_SANITIZER_UNDEFINED
       OR kbase_ENABLE_SANITIZER_ADDRESS
       OR kbase_ENABLE_SANITIZER_THREAD
       OR kbase_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    kbase_enable_hardening(kbase_options OFF
                             ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()
