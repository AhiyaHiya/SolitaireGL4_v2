#pragma once

#include "Types.hpp"

// clang-format off
#include <glad/gl.h>
#include <glm/glm.hpp>
// clang-format on

#include <expected>
#include <vector>

struct CardRenderer
{
    GLuint shader_program = 0;
    GLuint texture_array  = 0; // Still fine to call it this, or rename to texture_atlas
    GLuint vao            = 0;
    GLuint vbo            = 0;

    GLint uProjection = -1;
    GLint uPosition   = -1;
    GLint uSize       = -1;
    GLint uTexRect    = -1; // This one is now used!

    std::vector<glm::vec4> uv_rects; // One per card (including back)
};

auto create_card_renderer(GLuint program_id, GLuint vao_id, GLuint vbo_id)
    -> std::expected<CardRenderer, std::string>;
