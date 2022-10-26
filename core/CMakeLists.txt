add_library(core ChatEvent.h ChatEvent.cpp ChatPacket.cpp ChatPacket.h CoreUtility.cpp CoreUtility.h ChatFilePacket.cpp ChatFilePacket.h FileManager.cpp FileManager.h Room.cpp Room.h ChatAppModel.cpp ChatAppModel.h User.cpp User.h ChatUserModel.cpp ChatUserModel.h ChatRoomModel.cpp ChatRoomModel.h AppEvent.cpp AppEvent.h)
target_link_libraries(core Qt5::Core)
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(core fmt)
target_include_directories(core PUBLIC ${FMT_INCLUDE_DIRS})

add_executable(FileManagerTests fileManagerTests.cpp)
target_link_libraries(FileManagerTests core)