project(
  kbase_app
  VERSION 0.1.0
  DESCRIPTION "Example executable application"
  LANGUAGES CXX)

#
# Set our source and include directory variables for use through the build
#
set(PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PACKAGE_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

#
# Add all source files
#
set(SOURCE_FILES "${PACKAGE_SOURCE_DIR}/main.cpp")

#
# Output build information
#
message(STATUS "Building ${PROJECT_NAME} in [${CMAKE_BUILD_TYPE}] mode")

if(kbase_ENABLE_DEVELOPER_MODE)
  message(STATUS "Tests active for ${PROJECT_NAME}")
  add_subdirectory(tests)
endif()

#
# Declare the executable and add all source modules
#
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE kbase_options
                                              kbase_warnings kbase::data_structures)

if(kbase_ENABLE_DEVELOPER_MODE)
  run_clang_format(${PACKAGE_SOURCE_DIR})
  run_clang_format(${PACKAGE_TEST_DIR})
endif()
