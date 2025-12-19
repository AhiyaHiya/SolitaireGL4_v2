#include "File.hpp"

#include <fstream>

auto read_file_content(const std::filesystem::path& file_path)
    -> std::expected<std::string, error_message_t>
{
    // Make sure the file exists
    if (!std::filesystem::exists(file_path))
    {
        return std::unexpected("File does not exist: " + file_path.string());
    }

    // Attempt to open the file
    auto file_stream = std::ifstream(file_path, std::ios::in);
    if (!file_stream.is_open())
    {
        return std::unexpected("Failed to open file: " + file_path.string());
    }
    // Seek to the end to get the size
    file_stream.seekg(0, std::ios::end);

    // Set up a new string object to hold the content
    const auto size    = file_stream.tellg();
    auto       content = std::string(size, '\0');

    // Bring the reader back to the beginning of the file and read the content
    file_stream.seekg(0);
    file_stream.read(content.data(), size);
    return content;
}
