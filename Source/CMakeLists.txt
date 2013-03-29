find_package(SFML REQUIRED COMPONENTS System Window Graphics)

include_directories(${SFML_INCLUDE_DIR})

set(MAIN_SOURCES main.cpp Game.cpp GameData.cpp Assets.cpp AbstractGameState.cpp TileMap.cpp)
set(UTILITY_SOURCES Utility/Direction.cpp Utility/StringUtilities.cpp Utility/SwitchParser.cpp)
set(DEPENDENCY_SOURCES Dependencies/base64.cpp Dependencies/tinyxml2.cpp)
set(BURGLARY_SOURCES BurglaryState/Light.cpp BurglaryState/Camera.cpp BurglaryState/Guard.cpp BurglaryState/BurglaryState.cpp BurglaryState/Entity.cpp BurglaryState/Loot.cpp BurglaryState/EntityManager.cpp BurglaryState/Player.cpp)

file(GLOB_RECURSE HEADERS "*.h")
add_executable(blackguard ${MAIN_SOURCES} ${UTILITY_SOURCES} ${DEPENDENCY_SOURCES} ${BURGLARY_SOURCES} ${HEADERS})
target_link_libraries(blackguard ${SFML_LIBRARIES})

install(TARGETS blackguard RUNTIME DESTINATION bin)