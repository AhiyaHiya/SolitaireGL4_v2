#pragma once

#include "Types.hpp"

#include <stb_image.h>

#include <cstdint>
#include <expected>
#include <memory>
#include <vector>

// For holding image data from the large PNG tile map
class AssetImage
{
  public:
    AssetImage(std::shared_ptr<stbi_uc> data,
               std::int32_t             width,
               std::int32_t             height,
               std::int32_t             channels)
        : m_data(data)
        , m_width(width)
        , m_height(height)
        , m_channels(channels)
    {
    }

    auto data() const -> std::shared_ptr<stbi_uc>
    {
        return m_data;
    }
    auto width() const -> std::int32_t
    {
        return m_width;
    }
    auto height() const -> std::int32_t
    {
        return m_height;
    }
    auto channels() const -> std::int32_t
    {
        return m_channels;
    }

    auto stride_of_data_at(const std::uint32_t x, const std::uint32_t y) const -> stbi_uc*
    {
        return m_data.get() + (y * m_width * m_channels) + (x * m_channels);
    }

  private:
    std::shared_ptr<stbi_uc> m_data;
    std::int32_t             m_width;
    std::int32_t             m_height;
    std::int32_t             m_channels;
};

/// ----------------------------------------------------------------
/// @brief Load the PNG data from the assets folder and return an AssetImage object
/// @return Expected shared pointer to AssetImage or error message
auto load_png_data() -> std::expected<std::shared_ptr<AssetImage>, error_message_t>;
