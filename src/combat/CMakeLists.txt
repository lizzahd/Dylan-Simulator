# -------- Library --------
add_library(combat_lib STATIC
        lib/src/Combat.cpp
)

target_link_libraries(combat_lib PRIVATE core_lib)

target_include_directories(combat_lib
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/lib/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/src
)

# -------- Ivan Executable --------
add_executable(combat_exec_ivan
        exec_ivan/IvanMain.cpp
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