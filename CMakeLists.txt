cmake_minimum_required(VERSION 3.24)

##
# vcpkg portion
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(bootstrap_vcpkg_ext "bat")
else()
    set(bootstrap_vcpkg_ext "sh")
endif()

execute_process(
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/vcpkg/bootstrap-vcpkg.${bootstrap_vcpkg_ext}"
        RESULT_VARIABLE bootstrap_vcpkg_result
)

if(NOT bootstrap_vcpkg_result EQUAL 0)
    message(FATAL_ERROR "Bootstrapping failed with code: ${bootstrap_vcpkg_result}")
else()
    message(STATUS "Bootstrapping succeeded.")
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
# End vcpkg portion
##

project(Solitaire LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_COMPILER "g++" CACHE STRING "C++ compiler" FORCE)
    set(CMAKE_C_COMPILER "gcc" CACHE STRING "C compiler" FORCE)
endif ()

# --------------------- Dependencies ---------------------
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# --------------------- CPM ---------------------
# Download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Now download the other headers/libraries
include(CMake/stb.cmake)
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:google/googletest@1.17.0")

# --------------------- Executable ---------------------
set(GAME_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(solitaire
        Game/Assets.cpp
        Game/Assets.hpp
        Game/CardRenderer.cpp
        Game/CardRenderer.hpp
        Game/Cards.cpp
        Game/Cards.hpp
        Game/File.cpp
        Game/File.hpp
        Game/JSON.cpp
        Game/JSON.hpp
        Game/main.cpp
        Game/Shaders.cpp
        Game/Shaders.hpp
        Game/Types.hpp
        Game/VertexBuffer.cpp
        Game/VertexBuffer.hpp
        Game/Window.cpp
        Game/Window.hpp
)

target_compile_features(solitaire PUBLIC cxx_std_23)
target_include_directories(solitaire PRIVATE ${GLAD_INCLUDE_DIRS})
target_link_libraries(solitaire
    PRIVATE
        glfw
        stb
        glad::glad
        glm::glm

        nlohmann_json::nlohmann_json
)

# Needed so that when app starts up, it can locate the shared libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_options(solitaire
        PRIVATE
            "-Wl,-rpath,/usr/local/gcc-15/lib64"
    )
endif()

# --------------------- Tests ---------------------
enable_testing()
include(GoogleTest)
add_subdirectory(Game/Tests)

# --------------------- Copy assets ---------------------
add_custom_command(
    TARGET solitaire POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:solitaire>/Assets
    COMMENT "Copying Assets folder to output directory"
)
