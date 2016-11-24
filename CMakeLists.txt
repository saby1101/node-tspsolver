cmake_minimum_required(VERSION 2.6)

project(node_tspsolver)

set(CMAKE_MACOSX_RPATH ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
