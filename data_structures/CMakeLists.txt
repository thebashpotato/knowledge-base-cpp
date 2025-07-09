project(
  data_structures
  VERSION 0.1.0
  DESCRIPTION "Data Structures"
  LANGUAGES CXX)

#
# Set our source and include directory variables for use through the build
#
set(PACKAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#
# Add all source files
#
set(PUBLIC_HEADERS
    "${PACKAGE_INCLUDE_DIR}/core.hpp" "${PACKAGE_INCLUDE_DIR}/node.hpp"
    "${PACKAGE_INCLUDE_DIR}/singley_linked_list.hpp"
    "${PACKAGE_INCLUDE_DIR}/queue.hpp"
    "${PACKAGE_INCLUDE_DIR}/stack.hpp")

#
# Output build information
#
message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")

#
# Add tests if in Debug mode
#
if(kbase_ENABLE_DEVELOPER_MODE)
  message(STATUS "Tests active for ${PROJECT_NAME}")
  add_subdirectory(tests)
endif()

#
# Create the library
#
add_library(${PROJECT_NAME} INTERFACE)
add_library(kbase::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#
# Add private sources
#
target_sources(${PROJECT_NAME} PRIVATE ${PRIVATE_SOURCES})

#
# Declare public header files (will also be made available to IDE) and version
# the library
#
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}"
             VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR})

# Link required dependencies
target_link_libraries(${PROJECT_NAME} INTERFACE kbase_options kbase_warnings)

target_include_directories(
  ${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${PACKAGE_INCLUDE_DIR}>
                            $<INSTALL_INTERFACE:include>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

if(kbase_ENABLE_DEVELOPER_MODE)
  run_clang_format(${PACKAGE_INCLUDE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
endif()
