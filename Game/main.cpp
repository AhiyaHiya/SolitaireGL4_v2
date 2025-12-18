// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h> // Ordering is important and this file must be included after glad
// clang-format on

#include "Window.hpp"

#include <iostream>

constexpr auto generic_error = 1;
constexpr auto no_error      = 0;

// ----------------------------------------------------------------
/// @brief No description needed
/// @return 0 for no error, everything else is error
int main()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Failed to initialize GLFW\n";
        return generic_error;
    }

    constexpr auto width  = 1400;
    constexpr auto height = 1000;

    auto window_result = create_window(width, height, "Solitaire");
    if (!window_result)
    {
        auto [error_code, error_string] = window_result.error();
        glfwTerminate();
        std::cerr << "Failed to create window" << error_string << "\n";
        std::cerr << "\tError code:" << error_code << "\n";
        std::cerr << "\tError msg:" << error_string << "\n";
        return generic_error;
    }
    auto window = window_result.value();
    glfwMakeContextCurrent(window.get());

    

    glfwTerminate();
    return no_error;
}
