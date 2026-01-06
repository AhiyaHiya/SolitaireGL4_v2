#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

using error_code_t    = std::int32_t;
using error_message_t = std::string;

struct Frame
{
    std::int32_t x;
    std::int32_t y;
    std::int32_t w;
    std::int32_t h;
};
using Frames = std::unordered_map<std::string, Frame>;
