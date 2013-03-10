find_package(SFML REQUIRED COMPONENTS System Window Graphics)

file(GLOB_RECURSE HEADERS "*.h")
include_directories(${SFML_INCLUDE_DIR})
add_executable(blackguard Dependencies/base64.cpp TileMap.cpp Map.cpp Player.cpp Entity.cpp Dependencies/tinyxml2.cpp Utility/SwitchParser.cpp WorldmapState/WorldmapState.cpp BurglaryState/BurglaryState.cpp AbstractGameState.cpp GameData.cpp Assets.cpp Game.cpp main.cpp ${HEADERS})
target_link_libraries(blackguard ${SFML_LIBRARIES})

install(TARGETS blackguard RUNTIME DESTINATION bin)