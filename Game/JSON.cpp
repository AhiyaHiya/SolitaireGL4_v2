#include "JSON.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <typeinfo>

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

    const auto& frames_object = json["frames"];
    const auto& frames_items  = frames_object.items();
    auto        frames        = Frames{};

    // index is important in that it gives us a known position in the atlas
    // which is needed for later OpenGL command calls to access data in uv_rects
    for (const auto& [index, item] : std::ranges::views::enumerate(frames_items))
    {
        const auto& key        = item.key();
        const auto& frame_data = item.value();

        auto frame = Frame{
            .x = frame_data["x"].get<std::int32_t>(),
            .y = frame_data["y"].get<std::int32_t>(),
            .w = frame_data["w"].get<std::int32_t>(),
            .h = frame_data["h"].get<std::int32_t>(),
        };
        frames[key] = std::make_pair(frame, index);
    }
    return frames;
}

auto load_json_data() -> std::expected<nlohmann::ordered_json, error_message_t>
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
        return nlohmann::ordered_json::parse(json_file_stream); // This could throw
    }
    catch (const std::exception& e)
    {
        return std::unexpected(std::string("Failed to parse JSON data: ") + e.what());
    }
}
