#pragma once

#include "Types.hpp"

// clang-format off
#include <glad/gl.h>
#include <glm/glm.hpp>
// clang-format on

#include <expected>
#include <memory>
#include <vector>

struct CardRenderer
{
    GLuint shader_program = 0;
    GLuint texture_array  = 0; // Still fine to call it this, or rename to texture_atlas
    GLuint vao            = 0;
    GLuint vbo            = 0;

    GLint uProjection   = -1;
    GLint uPosition     = -1;
    GLint uSize         = -1;
    GLint uTexRect      = -1; // This one is now used!
    GLint uDebugOverlay = -1;
    GLint uDebugColor   = -1;

    std::vector<glm::vec4> uv_rects; // One per card (including back)
};

struct DrawCommand
{
    glm::vec2 position;   // center in pixels
    glm::vec2 size;       // e.g., {120, 180}
    int       card_index; // 0..52 (52 = back)
};

class AssetImage;
// flip_y: if true, convert JSON top-left Y coordinates to OpenGL bottom-left UVs
auto create_card_textures(std::shared_ptr<AssetImage> asset_image, const Frames& frames)
    -> std::pair<GLuint, std::vector<glm::vec4>>;

auto create_initial_draw_commands(
    const Frames& frames, const std::unordered_map<std::string, atlas_pos_t>& frame_positions)
    -> std::vector<DrawCommand>;

// Draws cards. If debug_overlay==true, the shader will output `debug_color` instead of
// sampling the texture, which is useful for verifying positions and sizes.
void draw_cards(const CardRenderer&             card_renderer,
                const std::vector<DrawCommand>& cards_to_draw,
                bool                            debug_overlay = false,
                const glm::vec4&                debug_color   = glm::vec4{0.0f, 1.0f, 0.0f, 0.5f});
