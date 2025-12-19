#pragma once

#include "Types.hpp"

#include <glad/gl.h>

#include <expected>
#include <utility>

/// ----------------------------------------------------------------
/// @brief Creates a VAO and VBO for rendering a textured quad
/// @return A std::expected containing the VAO and VBO IDs or an error message
auto create_vao_vbo() -> std::expected<std::pair<GLuint, GLuint>, error_message_t>;
