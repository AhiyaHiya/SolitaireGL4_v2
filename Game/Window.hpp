#pragma once

#include "Types.hpp"

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <expected>
#include <memory>
#include <string_view>

struct GLFWwindow;

/// @brief Creates a GLFW window with the specified dimensions and title.
/// @return Either a shared_ptr or a pair containing an error code and message.
auto create_window(std::int32_t width, std::int32_t height, std::string_view title)
    -> std::expected<std::shared_ptr<GLFWwindow>, std::pair<error_code_t, error_message_t>>;
