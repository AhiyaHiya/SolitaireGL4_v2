#include "VertexBuffer.hpp"

#include <array>

auto create_vao_vbo() -> std::expected<std::pair<GLuint, GLuint>, error_message_t>
{
    // For both VAO and VBO, there is only 1 item being created
    constexpr auto item_count = GLsizei{1};

    auto vao_id = GLuint();
    glGenVertexArrays(item_count, &vao_id);
    glBindVertexArray(vao_id);
    auto gl_error = glGetError();
    if (gl_error != GL_NO_ERROR || vao_id == 0)
    {
        return std::unexpected("Failed to create VAO: " + std::to_string(gl_error));
    }

    auto vbo_id = GLuint();
    glGenBuffers(item_count, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    gl_error = glGetError();
    if (gl_error != GL_NO_ERROR || vbo_id == 0)
    {
        glDeleteVertexArrays(1, &vao_id);
        glBindVertexArray(0);
        return std::unexpected("Failed to create VBO: " + std::to_string(gl_error));
    }

    auto vertices = std::array<GLfloat, 30>{
        // pos x   y    z    u    v
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // TL
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // TR
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // BR

        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // TL (repeat)
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // BR (repeat)
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f  // BL
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    constexpr auto position_attribute_index = GLuint(0);
    constexpr auto position_axis_count      = 3;
    constexpr auto column_count             = 5;
    glVertexAttribPointer(position_attribute_index,
                          GLint(position_axis_count),              // position_size,
                          GLenum(GL_FLOAT),                        // position_type,
                          GLboolean(GL_FALSE),                     // position_normalized,
                          GLsizei(column_count * sizeof(GLfloat)), // position_stride,
                          nullptr);                                // position_offset
    glEnableVertexAttribArray(position_attribute_index);

    // TexCoord attribute
    constexpr auto texcoord_attribute_index = GLuint(1);
    constexpr auto texcoord_axis_count      = 2;
    const auto     texcoord_offset          = GLintptr(position_axis_count * sizeof(GLfloat));
    glVertexAttribPointer(texcoord_attribute_index,
                          GLint(texcoord_axis_count),              // texcoord_size,
                          GLenum(GL_FLOAT),                        // texcoord_type,
                          GLboolean(GL_FALSE),                     // texcoord_normalized,
                          GLsizei(column_count * sizeof(GLfloat)), // texcoord_stride,
                          reinterpret_cast<GLvoid*>(texcoord_offset));
    glEnableVertexAttribArray(texcoord_attribute_index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return std::make_pair(vao_id, vbo_id);
}
