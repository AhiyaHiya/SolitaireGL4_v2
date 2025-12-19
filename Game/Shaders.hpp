#pragma once

#include "Types.hpp"

// clang-format off
#include <glad/gl.h>
#include <glm/glm.hpp>
// clang-format on

#include <expected>
#include <filesystem>

/// @brief Pass in relative path of shader file, and function will attempt to compile it
/// @param shader_path e.g. "Shaders/card.vert"
/// @param shader_type e.g. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
/// @return The id of the shader compiled or an error message
auto compile_card_shader(const std::filesystem::path& shader_path, GLenum shader_type)
    -> std::expected<GLuint, error_message_t>;
