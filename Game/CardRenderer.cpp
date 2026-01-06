#include "CardRenderer.hpp"

#include "Assets.hpp"
#include "Shaders.hpp"
#include "VertexBuffer.hpp"

#include <ranges>

auto create_card_textures(std::shared_ptr<AssetImage> asset_image, const Frames& frames)
    -> std::pair<GLuint, std::vector<glm::vec4>>
{
    auto           texture_id    = GLuint();
    constexpr auto texture_count = GLsizei(1);
    glGenTextures(texture_count, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, // target
                 0,             // level
                 GL_RGBA8,      // internal format
                 asset_image->width(),
                 asset_image->height(),
                 0, // border
                 GL_RGBA,
                 GL_UNSIGNED_BYTE, // type of stbi_uc
                 asset_image->data().get());

    auto uv_rects = std::vector<glm::vec4>{};
    uv_rects.reserve(frames.size());

    for (const auto& [frame_name, item] : frames)
    {
        auto min_u   = static_cast<float>(item.x) / asset_image->width();
        auto min_v   = static_cast<float>(item.y) / asset_image->height();
        auto scale_u = static_cast<float>(item.w) / asset_image->width();
        auto scale_v = static_cast<float>(item.h) / asset_image->height();

        uv_rects.emplace_back(min_u, min_v, scale_u, scale_v);
    }

    return std::make_pair(texture_id, uv_rects);
}

auto create_initial_draw_commands(const Frames& frames) -> std::vector<DrawCommand>
{
    auto cards = std::vector<DrawCommand>();
    cards.reserve(52 + 1); // 52 cards + 1 back

    for (auto const [index, item] : std::views::enumerate(frames))
    {
        const auto& frame = item.second;

        const auto width    = static_cast<float>(frame.w);
        const auto height   = static_cast<float>(frame.h);
        const auto x        = static_cast<float>(frame.x);
        const auto y        = static_cast<float>(frame.y);
        const auto center_x = x + width / 2.0f;
        const auto center_y = y + height / 2.0f;

        cards.push_back(DrawCommand{
            .position   = glm::vec2{center_x, center_y},
            .size       = glm::vec2{width, height},
            .card_index = static_cast<int>(index),
        });
    }
    return cards;
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
