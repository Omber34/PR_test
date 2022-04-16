
set ( RSC
        ../client/qml.qrc
        )

QT5_ADD_RESOURCES(RSCS ${RSC})
add_library(ChatClientLib ChatClient.cpp ChatClient.h ChatModel.cpp ChatModel.h ClientFileManager.cpp ClientFileManager.h ChatClientImpl.h)
target_include_directories(ChatClientLib PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ChatClientLib core)
target_link_libraries(ChatClientLib ${Boost_LIBRARIES})
target_include_directories(ChatClientLib PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(ChatClientLib Qt5::Qml Qt5::Quick)
target_include_directories(ChatClientLib PUBLIC ../core)

add_executable(ChatClient client.cpp ${RSCS})
target_link_libraries(ChatClient ChatClientLib)
