cmake_minimum_required (VERSION 2.6)
project("Dark Domains of Space")

# To find SFML headers
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/SFML/cmake/Modules")
# Set an output directory for our binaries
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# Typing every file separately is actually what CMake itself recommends. The
# problem with GLOB is that when a new file is added, cmake has to be rerun
# manually. We believe this won't be too difficult to keep in mind, so we
# use GLOB.
#add_executable(ddos main.cpp
#                    audio.cpp
#                    light.cpp
#                    map.cpp
#                    mapgenerator.cpp
#                    menu.cpp
#                    object.cpp
#                    pathfinding.cpp
#                    projectile.cpp
#                    random.cpp
#                    statemanager.cpp
#                    weapon.cpp)

file(GLOB SOURCES src/*.cpp)
add_executable(ddos ${SOURCES})
add_definitions(-std=c++11 -g -Wall -Wextra -pedantic)
find_package(SFML 2 REQUIRED system window graphics network audio)
target_link_libraries(ddos ${SFML_LIBRARIES})
install(TARGETS ddos RUNTIME DESTINATION ${BIN_DIR})
