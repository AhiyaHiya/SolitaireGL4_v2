#include "CardRenderer.hpp"

#include "Assets.hpp"
#include "Cards.hpp"
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

    const auto asset_image_width  = asset_image->width();
    const auto asset_image_height = asset_image->height();
    glTexImage2D(GL_TEXTURE_2D, // target
                 0,             // level
                 GL_RGBA8,      // internal format
                 asset_image_width,
                 asset_image_height,
                 0, // border
                 GL_RGBA,
                 GL_UNSIGNED_BYTE, // type of stbi_uc
                 asset_image->data().get());

    // Each entry in the uv_rects matches the images as they are laid out on the atlas sprite sheet
    auto uv_rects = std::vector<glm::vec4>{};
    uv_rects.reserve(frames.size());

    // Frames is not ordered by atlas position, so we need to order the
    // values before inserting them into the uv_rects vector
    auto ordered_frames = std::array<glm::vec4, 53>{};
    for (const auto& [frame_name, value_pair] : frames)
    {
        const auto& [frame, pos] = value_pair;
        auto min_u               = static_cast<float>(frame.x) / asset_image_width;
        // JSON frame coordinates use an origin in the upper-left of the image.
        // Convert to OpenGL texture coordinates which use a lower-left origin by
        // flipping the Y coordinate: new_y = image_height - (y + h)
        auto min_v_f = static_cast<float>(asset_image_height - frame.y - frame.h) /
                       static_cast<float>(asset_image_height);
        auto scale_u = static_cast<float>(frame.w) / asset_image_width;
        auto scale_v = static_cast<float>(frame.h) / asset_image_height;

        ordered_frames[pos] = glm::vec4{min_u, min_v_f, scale_u, scale_v};
    }
    for (const auto& rect : ordered_frames)
    {
        uv_rects.push_back(rect);
    }

    return std::make_pair(texture_id, uv_rects);
}

/*
 Bind items within a 1400x1000 window
 All images are width 256, and height 372
*/
auto create_initial_draw_commands(
    const Frames& frames, const std::unordered_map<std::string, atlas_pos_t>& frame_positions)
    -> std::vector<DrawCommand>
{
    auto cards = std::vector<DrawCommand>();
    cards.reserve(52); // 52 cards

    constexpr auto window_width  = 1400.0f;
    constexpr auto window_height = 1000.0f;

    const auto& first_card   = frames.begin()->second.first;
    const auto  atlas_card_w = static_cast<float>(first_card.w);
    const auto  atlas_card_h = static_cast<float>(first_card.h);

    constexpr auto cols = 13;
    constexpr auto rows = 4;

    const auto aspect = atlas_card_h / atlas_card_w;

    // Fit cards to window, preserving aspect ratio
    // Scale to fit vertically
    const auto max_w_by_cols = window_width / static_cast<float>(cols);
    const auto max_w_by_rows = (window_height / static_cast<float>(rows)) / aspect;
    const auto card_w        = std::min(max_w_by_cols, max_w_by_rows);
    const auto card_h        = card_w * aspect;

    // Center the grid inside the window
    const auto grid_w   = static_cast<float>(cols) * card_w;
    const auto grid_h   = static_cast<float>(rows) * card_h;
    const auto margin_x = (window_width - grid_w) / 2.0f;
    const auto margin_y = (window_height - grid_h) / 2.0f;
#if (1)
    auto i = 0;

    // Take only the first 52 frames (skip the 53rd which is the card back)
    const auto& deck_file_names = get_deck_file_names();
    for (const auto& card_name : deck_file_names | std::views::take(52))
    {
        const auto r = i / cols;
        const auto c = i % cols;

        const auto center_x = margin_x + static_cast<float>(c) * card_w + card_w / 2.0f;
        // const auto center_y = margin_y + static_cast<float>(r) * card_h + card_h / 2.0f;
        //  Invert y to start row 0 at top
        const auto center_y =
            window_height - (margin_y + static_cast<float>(r) * card_h + card_h / 2.0f);

        const auto idx = static_cast<int>(frame_positions.at(card_name));
        cards.push_back(DrawCommand{
            .position   = glm::vec2{center_x, center_y},
            .size       = glm::vec2{card_w, card_h},
            .card_index = idx, // index in the uv_rects container
        });
        ++i;
    }
#else
    for (auto i = 0; i < 52; ++i)
    {
        const auto r = i / cols;
        const auto c = i % cols;

        const auto center_x = margin_x + static_cast<float>(c) * card_w + card_w / 2.0f;
        const auto center_y =
            window_height - (margin_y + static_cast<float>(r) * card_h + card_h / 2.0f);

        auto index = i < 4 ? i : 51;
        cards.push_back(DrawCommand{
            // Where you are drawing on screen
            .position = glm::vec2{center_x, center_y},
            .size     = glm::vec2{card_w, card_h},
            // Coordinates of image data on the loaded sprite
            .card_index = index, // index in the uv_rects container
        });
    }
#endif
    return cards;
}

void draw_cards(const CardRenderer&             card_renderer,
                const std::vector<DrawCommand>& cards_to_draw,
                bool                            debug_overlay,
                const glm::vec4&                debug_color)
{
    glUseProgram(card_renderer.shader_program);
    glBindVertexArray(card_renderer.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, card_renderer.texture_array);

    for (const auto& cmd : cards_to_draw)
    {
        // Position and size in screen pixels (projection handles conversion)
        glUniform2f(card_renderer.uPosition, cmd.position.x, cmd.position.y);
        glUniform2f(card_renderer.uSize, cmd.size.x, cmd.size.y);

        // If debug overlay is requested, set debug uniforms; the fragment shader
        // will output uDebugColor instead of sampling the texture.
        if (card_renderer.uDebugOverlay != -1)
        {
            glUniform1i(card_renderer.uDebugOverlay, debug_overlay ? GL_TRUE : GL_FALSE);
        }
        if (card_renderer.uDebugColor != -1)
        {
            glUniform4f(card_renderer.uDebugColor,
                        debug_color.r,
                        debug_color.g,
                        debug_color.b,
                        debug_color.a);
        }

        const auto& rect = card_renderer.uv_rects[cmd.card_index];
        glUniform4f(card_renderer.uTexRect, rect.x, rect.y, rect.z, rect.w);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
