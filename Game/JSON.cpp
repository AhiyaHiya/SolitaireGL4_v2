#include "JSON.hpp"

#include <filesystem>
#include <fstream>

auto create_frames() -> std::expected<Frames, error_message_t>
{
    auto json_result = load_json_data();
    if (!json_result)
    {
        return std::unexpected(json_result.error());
    }
    auto json = json_result.value();
    if (!json.contains("frames"))
    {
        return std::unexpected("JSON is missing the top-level 'frames' object");
    }
    auto& frames_object = json["frames"];
    try
    {
        auto frames = frames_object.get<Frames>();
        return frames;
    }
    catch (const nlohmann::json::exception& e)
    {
        return std::unexpected(std::string("Failed to parse frames data: ") + e.what());
    }
}

auto load_json_data() -> std::expected<nlohmann::json, error_message_t>
{
    // Load JSON data from file
    const auto current_working_path = std::filesystem::current_path();
    const auto cards_path           = current_working_path / "Assets/Cards.json";
    if (!std::filesystem::exists(cards_path))
    {
        return std::unexpected(std::string("JSON file does not exist at path: ") +
                               cards_path.string());
    }

    auto json_file_stream = std::ifstream(cards_path.string());
    try
    {
        return nlohmann::json::parse(json_file_stream); // This could throw
    }
    catch (const std::exception& e)
    {
        return std::unexpected(std::string("Failed to parse JSON data: ") + e.what());
    }
}
