#pragma once

#include "Types.hpp"

// clang-format off
#include <glad/gl.h>
#include <glm/glm.hpp>
// clang-format on

#include <expected>
#include <filesystem>

/// ----------------------------------------------------------------
/// @brief Pass in relative path of shader file, and function will attempt to compile it
/// @param shader_path e.g. "Shaders/card.vert"
/// @param shader_type e.g. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
/// @return The id of the shader compiled or an error message
auto compile_shader(const std::filesystem::path& shader_path, GLenum shader_type)
    -> std::expected<GLuint, error_message_t>;

/// ----------------------------------------------------------------
/// @brief Attempts to compile vertext and fragment shaders
/// @return Returns <vertext id and fragment id> or an error message
auto compile_shaders() -> std::expected<std::pair<GLuint, GLuint>, error_message_t>;

/// ----------------------------------------------------------------
/// @brief Attempts to create a shader program
/// @return The program id or an error message
auto create_program() -> std::expected<GLuint, error_message_t>;
