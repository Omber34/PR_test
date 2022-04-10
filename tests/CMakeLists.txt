cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

add_executable(core_tests  core_tests.cpp)
target_include_directories(core_tests PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(core_tests PUBLIC ../core)

target_link_libraries(core_tests gtest gtest_main core)