cmake_minimum_required(VERSION 4.1)
project(TTRPG)

set(CMAKE_CXX_STANDARD 23)

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

add_executable(
        TTRPG Main.cpp
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

target_link_libraries(TTRPG PRIVATE hot_assets)

target_compile_definitions(TTRPG PRIVATE
    ENTITY_TYPES_HEADER=<EntityTypes.h>
    ENTITY_MANAGER_EXTERNAL_HEADER=<EntityManagerExternal.h>
)
target_link_libraries(TTRPG PRIVATE hot_entities)

find_path(RAYLIB_CPP_INCLUDE_DIRS "raylib-cpp.hpp")
target_include_directories(TTRPG PRIVATE
        ${RAYLIB_CPP_INCLUDE_DIRS}
        characters/include
        core/include
        environment/include
)

find_package(raylib CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(TTRPG PRIVATE
        raylib
        nlohmann_json::nlohmann_json
)