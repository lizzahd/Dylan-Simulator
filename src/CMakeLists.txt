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

add_library(game_core STATIC
        core/src/Actor.cpp
        core/src/AStar.cpp
        environment/src/Map.cpp
        core/src/Utils.cpp
        characters/src/Character.cpp
        characters/include/Character.h
        characters/src/Player.cpp
        characters/include/Player.h
        core/src/Animation.cpp
        core/include/Animation.h
        environment/src/Interactable.cpp
        core/src/StyledText.cpp
        core/include/StyledText.h
        core/src/GameManager.cpp
        core/include/GameManager.h
        characters/src/Npc.cpp
        characters/include/Npc.h
        core/src/Dialogue.cpp
        core/include/Dialogue.h
)

target_link_libraries(game_core PUBLIC hot_assets)

target_compile_definitions(game_core PUBLIC
    ENTITY_TYPES_HEADER=<EntityTypes.h>
    ENTITY_MANAGER_EXTERNAL_HEADER=<EntityManagerExternal.h>
)
target_link_libraries(game_core PUBLIC hot_entities)

find_path(RAYLIB_CPP_INCLUDE_DIRS "raylib-cpp.hpp")
target_include_directories(game_core PUBLIC
        ${RAYLIB_CPP_INCLUDE_DIRS}
        characters/include
        core/include
        environment/include
)

find_package(raylib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(game_core PUBLIC
        raylib
        nlohmann_json::nlohmann_json
)

# -------- Game --------
add_executable(game
        Main.cpp
)

target_link_libraries(game PRIVATE game_core)

# -------- Level Editor --------
add_executable(level_editor
        level_editor/LevelEditor.cpp
        level_editor/Button.cpp
        level_editor/Button.h
)

target_link_libraries(level_editor PRIVATE game_core)
