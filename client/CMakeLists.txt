
set ( RSC
        ../client/qml.qrc
        )

QT5_ADD_RESOURCES(RSCS ${RSC})

add_executable(ChatClient client.cpp ChatClient.cpp ChatClient.h ChatModel.cpp ChatModel.h ${RSCS} ClientFileManager.cpp ClientFileManager.h)
target_link_libraries(ChatClient core)
target_link_libraries(ChatClient ${Boost_LIBRARIES})
target_include_directories(ChatClient PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(ChatClient Qt5::Qml Qt5::Quick)
target_include_directories(ChatClient PUBLIC ../core)