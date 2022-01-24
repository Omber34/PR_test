add_library(core ChatEvent.h ChatEvent.cpp ChatPacket.cpp ChatPacket.h CoreUtility.cpp CoreUtility.h ChatFilePacket.cpp ChatFilePacket.h FileManager.cpp FileManager.h)
target_link_libraries(core Qt5::Core)
target_include_directories(core PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(FileManagerTests fileManagerTests.cpp)
target_link_libraries(FileManagerTests core)