project(grg)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(SFML REQUIRED COMPONENTS System Window Graphics)

include(Macros.cmake)
ENABLE_CPP11()
include_directories(${SFML_INCLUDE_DIR})
add_executable(grg Game.cpp main.cpp Game.cpp)
target_link_libraries(grg ${SFML_LIBRARIES})

install(TARGETS grg RUNTIME DESTINATION bin)
