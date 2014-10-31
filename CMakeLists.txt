cmake_minimum_required(VERSION 2.6)
project(mxml)

add_subdirectory(lxml)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB MXML_SRC "src/*.cpp" "src/dom/*.cpp" "src/geometry/*.cpp" "src/parsing/*.cpp")

find_package (libxml2 REQUIRED)

include_directories(${LIBXML2_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lxml/src ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(mxml ${MXML_SRC})


# Tests
enable_testing()
file(GLOB TESTS_SRC "test/*.cpp")
add_executable(mxml_tester ${TESTS_SRC})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
target_link_libraries(mxml_tester lxml mxml ${LIBXML2_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(
	NAME mxml_tester
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/mxml_tester
)
