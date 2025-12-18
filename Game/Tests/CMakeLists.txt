add_executable(SolitaireTests
    JSONTests.cpp
    "${GAME_BASE_DIRECTORY}/Game/JSON.cpp"
)

gtest_add_tests(TARGET SolitaireTests)

target_include_directories(SolitaireTests
    PRIVATE
        "${GAME_BASE_DIRECTORY}/Game"
)

target_link_libraries(SolitaireTests
    PRIVATE
        glad
        gtest
        gtest_main
        stb

        nlohmann_json::nlohmann_json
)

target_link_options(SolitaireTests
    PRIVATE
        "-Wl,-rpath,/usr/local/gcc-15/lib64"
)

# --------------------- Copy assets ---------------------
add_custom_command(
    TARGET SolitaireTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${GAME_BASE_DIRECTORY}/Assets
            $<TARGET_FILE_DIR:SolitaireTests>/Assets
    COMMENT "Copying Assets folder to output directory"
)

