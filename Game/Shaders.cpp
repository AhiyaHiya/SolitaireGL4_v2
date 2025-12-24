#include "Shaders.hpp"
#include "File.hpp"

auto compile_shader(const std::filesystem::path& shader_path, GLenum shader_type)
    -> std::expected<GLuint, error_message_t>
{
    auto current_working_path = std::filesystem::current_path();
    auto shader_full_path     = current_working_path / shader_path;

    auto content_result = read_file_content(shader_full_path);
    if (!content_result.has_value())
    {
        return std::unexpected("Failed to read shader file: " + std::string(shader_full_path));
    }
    auto shader_source = content_result.value();
    if (shader_source.empty())
    {
        return std::unexpected("Shader source is empty for file: " + std::string(shader_full_path));
    }

    auto           shader_id      = glCreateShader(shader_type);
    constexpr auto shader_count   = 1;
    const GLchar*  shader_src_ptr = shader_source.c_str();
    glShaderSource(shader_id, shader_count, &shader_src_ptr, nullptr);
    glCompileShader(shader_id);

    // Check compilation status
    auto compile_status = GLint(0);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE)
    {
        // Get the error message
        auto       log_length  = GLsizei(0);
        const auto buffer_size = GLsizei(1024);
        auto       info_log    = std::string(buffer_size, '\0');
        glGetShaderInfoLog(shader_id, buffer_size, &log_length, info_log.data());

        // Delete the shader since we have a failed status
        glDeleteShader(shader_id);
        return std::unexpected("Shader compilation failed: " + info_log);
    }
    return shader_id;
}

auto compile_shaders() -> std::expected<std::pair<GLuint, GLuint>, error_message_t>
{
    auto vertex_compiled_result = compile_shader("Shaders/card.vert", GL_VERTEX_SHADER);
    if (!vertex_compiled_result)
    {
        return std::unexpected("Failed to compile vertex shader: " +
                               vertex_compiled_result.error());
    }
    auto vertex_shader_id = vertex_compiled_result.value();

    auto fragment_compiled_result = compile_shader("Shaders/card.frag", GL_FRAGMENT_SHADER);
    if (!fragment_compiled_result)
    {
        return std::unexpected("Failed to compile fragment shader: " +
                               fragment_compiled_result.error());
    }
    auto fragment_shader_id = fragment_compiled_result.value();

    return std::make_pair(vertex_shader_id, fragment_shader_id);
}

auto create_program() -> std::expected<GLuint, error_message_t>
{
    auto shaders_result = compile_shaders();
    if (!shaders_result)
    {
        return std::unexpected("Failed to compile shaders: " + shaders_result.error());
    }
    auto [vertex_shader_id, fragment_shader_id] = shaders_result.value();

    auto program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    auto program_linked = GLint();
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        // Get the size of the error message
        auto log_length = GLsizei(0);
        glGetProgramInfoLog(program_id, 0, &log_length, nullptr);

        auto err_message = std::string(log_length, '\0');
        glGetProgramInfoLog(program_id, log_length, &log_length, err_message.data());
        return std::unexpected(err_message);
    }

    return program_id;
}
