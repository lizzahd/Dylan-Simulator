# -------- Library --------
add_library(combat_lib STATIC
        lib/src/Combat.cpp
        lib/src/Character.cpp
        lib/include/Character.h
        lib/src/Player.cpp
        lib/include/Player.h
        lib/src/Enemy.cpp
        lib/include/Enemy.h
        lib/include/Projectile.h
        lib/src/Projectile.cpp
        lib/src/UI.cpp
)

target_link_libraries(combat_lib PRIVATE core_lib)

target_include_directories(combat_lib
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/lib/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/src
)

# -------- Ivan Executable --------
add_executable(combat_exec_ivan
        exec_ivan/IvanMain.cpp
        lib/include/UI.h
)

target_link_libraries(combat_exec_ivan PRIVATE
        core_lib
        combat_lib
)

# -------- Jens Executable --------
add_executable(combat_exec_jens
        exec_jens/JensMain.cpp
)

target_link_libraries(combat_exec_jens PRIVATE
        core_lib
        combat_lib
)