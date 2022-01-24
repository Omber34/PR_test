cmake_minimum_required(VERSION 3.19)
project(PR_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/boost/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/boost/lib")

find_package(Boost)

find_package(Qt5 COMPONENTS Core Qml Quick REQUIRED)

add_subdirectory(core)
add_subdirectory(client)
add_subdirectory(server)




