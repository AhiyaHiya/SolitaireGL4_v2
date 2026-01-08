add_executable(SolitaireTests
    "${GAME_BASE_DIRECTORY}/Game/File.cpp"
    "${GAME_BASE_DIRECTORY}/Game/JSON.cpp"
    FileTests.cpp
    JSONTests.cpp
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

# Ensure the test target is built with debug symbols and without optimizations
if (MSVC)
    # Disable optimizations and enable debug info for MSVC
    target_compile_options(SolitaireTests PRIVATE /Od /Zi)
    target_link_options(SolitaireTests PRIVATE /DEBUG)
else()
    # For GCC/Clang: disable optimization and include debug symbols
    target_compile_options(SolitaireTests PRIVATE -O0 -g)
endif()

# --------------------- Copy assets ---------------------
add_custom_command(
    TARGET SolitaireTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${GAME_BASE_DIRECTORY}/Assets
            $<TARGET_FILE_DIR:SolitaireTests>/Assets
    COMMENT "Copying Assets folder to output directory"
)

# --------------------- Copy test data ---------------------
add_custom_command(
    TARGET SolitaireTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${GAME_BASE_DIRECTORY}/Game/Tests/TestData
            $<TARGET_FILE_DIR:SolitaireTests>/Game/Tests/TestData
    COMMENT "Copying Game/Tests/TestData folder to output directory"
)

