function(kbase_enable_coverage kbase_name)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES
                                             ".*Clang")

    target_compile_options(${kbase_name} INTERFACE --coverage -O0 -g)
    target_link_libraries(${kbase_name} INTERFACE --coverage)
  endif()
endfunction()
