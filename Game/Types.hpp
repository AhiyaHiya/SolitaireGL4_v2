#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

using error_code_t    = std::int32_t;
using error_message_t = std::string;

using atlas_pos_t = std::int32_t;

struct Frame
{
    std::int32_t x;
    std::int32_t y;
    std::int32_t w;
    std::int32_t h;
};
using Frames = std::unordered_map<std::string, std::pair<Frame, atlas_pos_t>>;

// <card name, position index>
using FramesIndex = std::unordered_map<std::string, std::int32_t>;
