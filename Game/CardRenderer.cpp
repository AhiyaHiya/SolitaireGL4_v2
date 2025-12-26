#include "CardRenderer.hpp"

#include "Shaders.hpp"
#include "VertexBuffer.hpp"

auto create_card_renderer(GLuint program_id, GLuint vao_id, GLuint vbo_id)
    -> std::expected<CardRenderer, std::string>
{
    return CardRenderer{
        .shader_program = program_id,
        .texture_array  = 0,
        .vao            = vao_id,
        .vbo            = vbo_id,

        .uProjection = glGetUniformLocation(program_id, "uProjection"),
        .uPosition   = glGetUniformLocation(program_id, "uPosition"),
        .uSize       = glGetUniformLocation(program_id, "uSize"),
        .uTexRect    = glGetUniformLocation(program_id, "uTexRect"),

        .uv_rects = std::vector<glm::vec4>{},
    };
}
