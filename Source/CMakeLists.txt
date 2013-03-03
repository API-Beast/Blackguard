find_package(SFML REQUIRED COMPONENTS System Window Graphics)

file(GLOB_RECURSE HEADERS "*.h")
include_directories(${SFML_INCLUDE_DIR})
add_executable(grg Utility/SwitchParser.cpp WorldmapState/WorldmapState.cpp BurglaryState/BurglaryState.cpp AbstractGameState.cpp GameData.cpp Assets.cpp Game.cpp main.cpp ${HEADERS})
target_link_libraries(grg ${SFML_LIBRARIES})

install(TARGETS grg RUNTIME DESTINATION bin)