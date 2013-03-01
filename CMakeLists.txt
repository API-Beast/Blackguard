cmake_minimum_required(VERSION 2.8)

project(grg)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
include(Macros.cmake)

# Which system? CMake should know best.
if(WIN32)
  set(GRG_WINDOWS 1)
elseif(APPLE)
  set(GRG_MAC 1)
elseif(UNIX)
  set(GRG_LINUX 1)
else()
  set(GRG_UNKNOWN 1)
endif()
option(GRG_PREFER_INSTALLED_VERSION "Load assets from installed version instead of source version if both are avaiable." OFF)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ENABLE_CPP11()

find_package(SFML REQUIRED COMPONENTS System Window Graphics)

include_directories(${SFML_INCLUDE_DIR})
add_executable(grg Assets.cpp Game.cpp main.cpp Game.cpp)
target_link_libraries(grg ${SFML_LIBRARIES})

install(TARGETS grg RUNTIME DESTINATION bin)