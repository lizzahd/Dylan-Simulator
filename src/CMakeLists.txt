cmake_minimum_required(VERSION 4.1)
project(TTRPG)

set(CMAKE_CXX_STANDARD 23)

# -------- Core Library --------
include(FetchContent)

FetchContent_Declare(
        hot_assets
        GIT_REPOSITORY https://github.com/lizzahd/Hot-Assets-Plus-Plus.git
        GIT_TAG "main"
)

FetchContent_MakeAvailable(hot_assets)

FetchContent_Declare(
        hot_entities
        GIT_REPOSITORY https://github.com/lizzahd/Hot-Entities-Plus-Plus.git
        GIT_TAG "main"
)

FetchContent_MakeAvailable(hot_entities)

add_library(core_lib STATIC
        core/src/Actor.cpp
        core/src/AStar.cpp
        overworld_map/src/Map.cpp
        core/src/Utils.cpp
        characters/src/Character.cpp
        characters/include/Character.h
        characters/src/Player.cpp
        characters/include/Player.h
        core/src/Animation.cpp
        core/include/Animation.h
        overworld_map/src/Interactable.cpp
        core/src/StyledText.cpp
        core/include/StyledText.h
        core/src/GameManager.cpp
        core/include/GameManager.h
        characters/src/Npc.cpp
        characters/include/Npc.h
        core/src/Dialogue.cpp
        core/include/Dialogue.h
)

target_link_libraries(core_lib PUBLIC hot_assets)

target_compile_definitions(core_lib PUBLIC
    ENTITY_TYPES_HEADER=<EntityTypes.h>
    ENTITY_MANAGER_EXTERNAL_HEADER=<EntityManagerExternal.h>
)
target_link_libraries(core_lib PUBLIC hot_entities)

find_path(RAYLIB_CPP_INCLUDE_DIRS "raylib-cpp.hpp")
target_include_directories(core_lib PUBLIC
        ${RAYLIB_CPP_INCLUDE_DIRS}
        characters/include
        core/include
        overworld_map/include
)

find_package(raylib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(core_lib PUBLIC
        raylib
        nlohmann_json::nlohmann_json
)

# -------- Main Game --------
add_executable(overworld_game
        Main.cpp
)

target_link_libraries(overworld_game PRIVATE core_lib)

# -------- Gauntlet Library --------
add_library(gauntlet_lib STATIC
        gauntlet/src/GauntletCharacter.cpp
        gauntlet/src/GauntletPlayer.cpp
        gauntlet/include/GauntletPlayer.h
        gauntlet/src/GauntletUtils.cpp
        gauntlet/include/GauntletUtils.h
)

target_link_libraries(gauntlet_lib PRIVATE core_lib)

target_include_directories(gauntlet_lib PUBLIC
        gauntlet/include
)

# -------- Gauntlet Game --------
add_executable(gauntlet_game
        gauntlet/GauntletMain.cpp
)

target_link_libraries(gauntlet_game PRIVATE
        core_lib
        gauntlet_lib
)

# -------- Level Editor --------
add_executable(level_editor
        level_editor/LevelEditor.cpp
        level_editor/Button.cpp
        level_editor/Button.h
)

target_link_libraries(level_editor PRIVATE core_lib)
