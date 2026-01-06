#include "CardRenderer.hpp"

#include "Assets.hpp"
#include "Shaders.hpp"
#include "VertexBuffer.hpp"

#include <algorithm>
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

/*
 Bind items within a 1400x1000 window
 All images are width 256, and height 372
*/
auto create_initial_draw_commands(const Frames& frames) -> std::vector<DrawCommand>
{
    auto cards = std::vector<DrawCommand>();
    cards.reserve(52); // 52 cards

    constexpr auto window_width  = 1400.0f;
    constexpr auto window_height = 1000.0f;

    const auto& first_card   = frames.begin()->second;
    const auto  atlas_card_w = static_cast<float>(first_card.w);
    const auto  atlas_card_h = static_cast<float>(first_card.h);

    constexpr auto cols = 13;
    constexpr auto rows = 4;

    const auto aspect = atlas_card_h / atlas_card_w;

    // Compute the maximum card width that will fit both horizontally and vertically
    const auto max_w_by_cols = window_width / static_cast<float>(cols);
    const auto max_w_by_rows = (window_height / static_cast<float>(rows)) / aspect;
    const auto card_w        = std::min(max_w_by_cols, max_w_by_rows);
    const auto card_h        = card_w * aspect;

    // Center the grid inside the window
    const auto grid_w   = cols * card_w;
    const auto grid_h   = rows * card_h;
    const auto margin_x = (window_width - grid_w) / 2.0f;
    const auto margin_y = (window_height - grid_h) / 2.0f;

    // Take only the first 52 frames (skip the 53rd which is the card back)
    for (auto const [index, item] : std::views::enumerate(frames) | std::views::take(52))
    {
        const int idx = static_cast<int>(index);
        const int r   = idx / cols;
        const int c   = idx % cols;

        const float center_x = margin_x + static_cast<float>(c) * card_w + card_w / 2.0f;
        const float center_y = margin_y + static_cast<float>(r) * card_h + card_h / 2.0f;

        cards.push_back(DrawCommand{
            .position   = glm::vec2{center_x, center_y},
            .size       = glm::vec2{card_w, card_h},
            .card_index = idx,
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
