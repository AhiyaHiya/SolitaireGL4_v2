// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h> // Ordering is important and this file must be included after glad
#include <glm/gtc/type_ptr.hpp>
// clang-format on

#include "Assets.hpp"
#include "CardRenderer.hpp"
#include "Cards.hpp"
#include "JSON.hpp"
#include "Shaders.hpp"
#include "VertexBuffer.hpp"
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

    // --------------------------- Create Window ---------------------------
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

    // --------------------------- Get OpenGL Version ---------------------------
    const auto opengl_version = gladLoadGL(glfwGetProcAddress);
    if (opengl_version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context\n";
        return generic_error;
    }
    printf("Loaded OpenGL %d.%d\n",
           GLAD_VERSION_MAJOR(opengl_version),
           GLAD_VERSION_MINOR(opengl_version));

    // --------------------------- Compile Program ---------------------------
    auto program_result = create_program();
    if (!program_result)
    {
        std::cerr << "Failed to create shader program: " << program_result.error() << "\n";
        return generic_error;
    }
    auto program_id = program_result.value();

    // Make program current so subsequent glUniform calls affect it
    glUseProgram(program_id);

    // --------------------------- Create buffers ---------------------------
    auto vao_vbo_result = create_vao_vbo();
    if (!vao_vbo_result)
    {
        std::cerr << "Failed to create VAO/VBO: " + vao_vbo_result.error() << "\n";
        return generic_error;
    }
    auto [vao_id, vbo_id] = vao_vbo_result.value();

    // --------------------------- Load Atlas ---------------------------
    auto atlas_result = load_png_data();
    if (!atlas_result)
    {
        std::cerr << "Failed to load PNG data: " << atlas_result.error() << "\n";
        return generic_error;
    }
    auto atlas = atlas_result.value();

    auto frames_result = create_frames();
    if (!frames_result)
    {
        std::cerr << "Failed to create frames from JSON: " << frames_result.error() << "\n";
        return generic_error;
    }
    auto frames                 = frames_result.value();
    auto [texture_id, uv_rects] = create_card_textures(atlas, frames);

    // --------------------------- Card Renderer ---------------------------
    auto card_renderer =
        CardRenderer{.shader_program = program_id,
                     .texture_array  = texture_id,
                     .vao            = vao_id,
                     .vbo            = vbo_id,

                     .uProjection = glGetUniformLocation(program_id, "uProjection"),
                     .uPosition   = glGetUniformLocation(program_id, "uPosition"),
                     .uSize       = glGetUniformLocation(program_id, "uSize"),
                     .uTexRect    = glGetUniformLocation(program_id, "uTexRect"),

                     .uv_rects = std::move(uv_rects)};

    // ...
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto projection = glm::ortho(0.0f, // left
                                 static_cast<float>(width),
                                 0.0f,                       // bottom → now 0
                                 static_cast<float>(height), // top   → now height
                                 -1.0f,
                                 1.0f);
    glUniformMatrix4fv(card_renderer.uProjection, // location
                       1,                         // count
                       GL_FALSE,                  // transpose
                       glm::value_ptr(projection));
    glUniform1i(glGetUniformLocation(card_renderer.shader_program, "uCardTextures"), 0);

    auto cards_to_draw = create_initial_draw_commands(frames);

    while (!glfwWindowShouldClose(window.get()))
    {
        // Boilerplate code for now
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Implement card drawing logic
        draw_cards(card_renderer, cards_to_draw);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    glfwTerminate();
    return no_error;
}
