# -------- Level Editor --------
add_executable(level_editor
        LevelEditor.cpp
        Button.cpp
        Button.h
)

target_link_libraries(level_editor PRIVATE core_lib)