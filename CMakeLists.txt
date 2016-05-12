cmake_minimum_required(VERSION 2.6)
project(mxml)

add_subdirectory(lxml)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB MXML_SRC "src/mxml/*.cpp" "src/mxml/dom/*.cpp" "src/mxml/geometry/*.cpp" "src/mxml/geometry/collisions/*.cpp" "src/mxml/parsing/*.cpp" "src/mxml/attributes/*.cpp")

find_package(libxml2 REQUIRED)

include_directories(SYSTEM ${LIBXML2_INCLUDE_DIR})
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/lxml/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(mxml ${MXML_SRC})


# Tests
enable_testing()
file(GLOB TESTS_SRC "tests/*.cpp")
add_executable(mxml_tester ${TESTS_SRC})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(mxml_tester lxml mxml ${LIBXML2_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(
	NAME mxml_tester
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests/resources
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/mxml_tester
)
