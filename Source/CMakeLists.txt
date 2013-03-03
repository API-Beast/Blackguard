if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG "/MTd")
  set(CMAKE_C_FLAGS_RELEASE "/MT")
  set(CMAKE_C_FLAGS_RELEASE "/Zi")
  set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "/MT")
  set(CMAKE_TARGET_DEFINITIONS "SFML_STATIC" "WIN32")
  add_definitions("/Zi /D \"SFML_STATIC\" /D \"_CRT_SECURE_NO_DEPRECATE\" ")
ENDIF(MSVC)

find_package(SFML REQUIRED COMPONENTS System Window Graphics)

file(GLOB_RECURSE HEADERS "*.h")
include_directories(${SFML_INCLUDE_DIR})
add_executable(grg GameData.cpp Assets.cpp Game.cpp main.cpp ${HEADERS})
target_link_libraries(grg ${SFML_LIBRARIES})

install(TARGETS grg RUNTIME DESTINATION bin)