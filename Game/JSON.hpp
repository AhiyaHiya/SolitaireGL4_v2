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

/// ----------------------------------------------------------------
/// @brief Use this function to create a unordered map of frames from the JSON file.
/// @return A std::expected containing the frames or an error message.
/// @remarks This function uses load_json_data() to read and parse the JSON file.
auto create_frames() -> std::expected<Frames, error_message_t>;

/// ----------------------------------------------------------------
/// @brief Load JSON data from the Cards.json file.
/// @return A std::expected containing the JSON data or an error message.
auto load_json_data() -> std::expected<nlohmann::json, error_message_t>;
