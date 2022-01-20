add_executable(ChatServer server.cpp ChatServer.cpp ChatServer.h)

target_link_libraries(ChatServer core)
target_link_libraries(ChatServer ${Boost_LIBRARIES})
target_link_libraries(ChatServer Qt5::Qml Qt5::Quick)

target_include_directories(ChatServer PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(ChatServer PUBLIC ../core)

