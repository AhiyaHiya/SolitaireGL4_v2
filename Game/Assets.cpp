#include "Assets.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>

auto load_png_data() -> std::expected<std::shared_ptr<AssetImage>, error_message_t>
{
    // Set up the asset path
    auto current_working_path = std::filesystem::current_path();
    auto cards_path           = current_working_path / "Assets/Cards.png";
    if (!std::filesystem::exists(cards_path))
    {
        return std::unexpected("Cards PNG file not found at path: " + cards_path.string());
    }

    stbi_set_flip_vertically_on_load(true);

    // Load the png data; width, height, and channels will be set by stbi_load
    auto  width = 0, height = 0, channels = 0;
    auto* raw_data = stbi_load(cards_path.string().c_str(), &width, &height, &channels, 0);
    if (raw_data == nullptr || width == 0 || height == 0 || channels == 0)
    {
        return std::unexpected("Failed to load PNG data from file: " + cards_path.string());
    }

    // Wrap the data and the image info in the asset_image object
    auto data_ptr = std::shared_ptr<stbi_uc>(raw_data, stbi_image_free);
    return std::make_shared<AssetImage>(data_ptr, width, height, channels);
}
