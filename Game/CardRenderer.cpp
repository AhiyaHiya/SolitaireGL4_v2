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

void draw_cards(const CardRenderer& card_renderer, const std::vector<DrawCommand>& cards_to_draw)
{
    glUseProgram(card_renderer.shader_program);
    glBindVertexArray(card_renderer.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, card_renderer.texture_array);

    for (const auto& cmd : cards_to_draw)
    {
        glUniform2f(card_renderer.uPosition, cmd.position.x, cmd.position.y);
        glUniform2f(card_renderer.uSize, cmd.size.x, cmd.size.y);

        const auto& rect = card_renderer.uv_rects[cmd.card_index];
        glUniform4f(card_renderer.uTexRect, rect.x, rect.y, rect.z, rect.w);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
