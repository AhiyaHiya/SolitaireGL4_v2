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

auto create_window(std::int32_t width, std::int32_t height, std::string_view title)
    -> std::expected<std::shared_ptr<GLFWwindow>, std::pair<error_code_t, error_message_t>>;