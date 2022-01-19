cmake_minimum_required(VERSION 3.19)
project(PR_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/boost/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/boost/lib")

find_package(Boost)

set ( RSC
        ./qml.qrc
        )

find_package(Qt5 COMPONENTS Core Qml Quick REQUIRED)

QT5_ADD_RESOURCES(RSCS ${RSC})

add_subdirectory(core)
add_subdirectory(client)
add_executable(ChatServer server.cpp ChatServer.cpp ChatServer.h)

target_link_libraries(ChatServer core)
target_link_libraries(ChatClient core)

target_link_libraries(ChatServer ${Boost_LIBRARIES})
target_include_directories(ChatServer PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ChatClient ${Boost_LIBRARIES})
target_include_directories(ChatClient PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(core Qt5::Core)
target_link_libraries(ChatClient Qt5::Qml Qt5::Quick)
target_link_libraries(ChatServer Qt5::Qml Qt5::Quick)
