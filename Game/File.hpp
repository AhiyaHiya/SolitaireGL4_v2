#pragma once

#include "Types.hpp"

#include <expected>
#include <filesystem>

/// @brief Will attempt to read the file at path and return its content as a string
/// @param file_path Full file path to read
/// @return The content of the file returned in a std::string or an error message
auto read_file_content(const std::filesystem::path& file_path)
    -> std::expected<std::string, error_message_t>;
