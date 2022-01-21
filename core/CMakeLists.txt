add_library(core ChatEvent.h ChatEvent.cpp ChatPacket.cpp ChatPacket.h CoreUtility.cpp CoreUtility.h)
target_link_libraries(core Qt5::Core)