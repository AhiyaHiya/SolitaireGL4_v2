#include "Shaders.hpp"

auto compile_shader(const std::filesystem::path& shader_path, GLenum shader_type)
    -> std::expected<GLuint, error_message_t>
{
    // Not implemented yet
    return std::unexpected("Not implemented yet");
}
