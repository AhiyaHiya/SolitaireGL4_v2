#pragma once

#include "Types.hpp"

#include <array>
#include <string>
#include <unordered_map>

auto get_deck_file_names() -> const std::array<std::string, 53>&;

// Creates a container where you can easily look up the atlas position by frame name
auto create_frames_position(const Frames& frames) -> std::unordered_map<std::string, atlas_pos_t>;
