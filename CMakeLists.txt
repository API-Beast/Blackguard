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

if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "/MTd")
  set(CMAKE_C_FLAGS_RELEASE "/MT")
  set(CMAKE_C_FLAGS_RELEASE "/Zi")
  set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "/MT")
	set(CMAKE_TARGET_DEFINITIONS "SFML_STATIC" "WIN32")
	add_definitions("/Zi /D \"SFML_STATIC\" /D \"_CRT_SECURE_NO_DEPRECATE\" ")
	file(GLOB_RECURSE INCS "*.h") # Forgive me but without MSVC won't list the headers :(
ENDIF(MSVC)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ENABLE_CPP11()

find_package(SFML REQUIRED COMPONENTS System Window Graphics)

include_directories(${SFML_INCLUDE_DIR})
add_executable(grg Assets.cpp Game.cpp main.cpp Game.cpp ${INCS})
target_link_libraries(grg ${SFML_LIBRARIES})

install(TARGETS grg RUNTIME DESTINATION bin)