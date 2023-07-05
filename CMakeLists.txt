#don't mess with this file, it deals with exes and the library
cmake_minimum_required(VERSION 2.8)
project(Caps)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_DEFAULT_SOURCE")
include_directories(include src)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(Caps_Exec main.c)
add_library(Caps STATIC src/segment.c src/packet.c src/packetForger.c src/socketManager.c src/packetInterpreter.c src/streamBuilder.c)

include(GNUInstallDirs)
include_directories(lib) 
target_link_libraries(Caps_Exec PUBLIC Caps)

install(TARGETS Caps_Exec RUNTIME DESTINATION bin)
install(TARGETS Caps ARCHIVE DESTINATION lib)
