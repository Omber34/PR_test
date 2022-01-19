
add_executable(ChatClient client.cpp ChatClient.cpp ChatClient.h ChatModel.cpp ChatModel.h)
target_link_libraries(ChatClient Qt5::Core Qt5::Qml Qt5::Quick)