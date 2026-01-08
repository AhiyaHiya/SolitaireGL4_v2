#pragma once

#include "Types.hpp"

#include <nlohmann/json.hpp>

#include <expected>
#include <string>

/// ----------------------------------------------------------------
/// @brief Use this function to create a unordered map of frames from the JSON file.
/// @return A std::expected containing the frames or an error message.
/// @remarks This function uses load_json_data() to read and parse the JSON file.
auto create_frames() -> std::expected<Frames, error_message_t>;

/// ----------------------------------------------------------------
/// @brief Load JSON data from the Cards.json file.
/// @return A std::expected containing the JSON data or an error message.
auto load_json_data() -> std::expected<nlohmann::ordered_json, error_message_t>;
