cmake_minimum_required(VERSION 2.8)

project(Blackguard)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
include(EnableCPP11Flag)

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

# On MSVC link the static STL version
if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "/MTd")
  set(CMAKE_C_FLAGS_RELEASE "/MT")
  set(CMAKE_C_FLAGS_RELEASE "/Zi")
  set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "/MT")
  set(CMAKE_TARGET_DEFINITIONS "SFML_STATIC" "WIN32")
  add_definitions("/Zi /D \"SFML_STATIC\" /D \"_CRT_SECURE_NO_DEPRECATE\" ")
ENDIF(MSVC)

add_subdirectory(Source)