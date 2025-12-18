#pragma once

#include "Types.hpp"

#include <nlohmann/json.hpp>

#include <expected>
#include <string>
#include <unordered_map>

struct Frame
{
    std::int32_t x;
    std::int32_t y;
    std::int32_t w;
    std::int32_t h;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Frame, x, y, w, h)

using Frames = std::unordered_map<std::string, Frame>;

auto create_frames() -> std::expected<Frames, error_message_t>;
auto load_json_data() -> std::expected<nlohmann::json, error_message_t>;
