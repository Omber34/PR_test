cmake_minimum_required(VERSION 3.19)
project(PR_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Qml Quick QuickControls2)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::QuickControls2)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)


SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/boost/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/boost/lib")

find_package(Boost)

set ( RSC
        ./qml.qrc
        )

QT5_ADD_RESOURCES(RSCS ${RSC})

add_subdirectory(core)
add_executable(ChatServer server.cpp ChatServer.cpp ChatServer.h)
add_executable(ChatClient client.cpp ChatClient.cpp ChatClient.h)

target_link_libraries(ChatServer core)
target_link_libraries(ChatClient core)

target_link_libraries(ChatServer ${Boost_LIBRARIES})
target_include_directories(ChatServer PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ChatClient ${Boost_LIBRARIES})
target_include_directories(ChatClient PRIVATE ${Boost_INCLUDE_DIRS})
