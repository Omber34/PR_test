cmake_minimum_required(VERSION 3.19)
project(PR_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/boost/include/boost")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files (x86)/googletest-distribution/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/boost/lib")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Program Files (x86)/googletest-distribution/lib")

find_package(Boost)

find_package(Qt5 COMPONENTS Core Qml Quick REQUIRED)

add_subdirectory(core)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)
add_subdirectory(lib)