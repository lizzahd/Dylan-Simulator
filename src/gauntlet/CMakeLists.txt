# -------- Gauntlet Library --------
add_library(gauntlet_lib STATIC
        src/GauntletCharacter.cpp
        src/GauntletPlayer.cpp
        include/GauntletPlayer.h
        src/GauntletUtils.cpp
        include/GauntletUtils.h
)

target_link_libraries(gauntlet_lib PRIVATE core_lib)

target_include_directories(gauntlet_lib
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -------- Gauntlet Game --------
add_executable(gauntlet_game
        GauntletMain.cpp
)

target_link_libraries(gauntlet_game PRIVATE
        core_lib
        gauntlet_lib
)