// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h> // Ordering is important and this file must be included after glad
// clang-format on

#include <iostream>

constexpr auto generic_error = 1;
constexpr auto no_error      = 0;

// ----------------------------------------------------------------
/// @brief No description needed
/// @return 0 for no error, everything else is error
int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return generic_error;
    }

    glfwTerminate();
    return no_error;
}
