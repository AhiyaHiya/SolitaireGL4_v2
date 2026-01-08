#include "JSON.hpp"

#include <gtest/gtest.h>

namespace {
static constexpr size_t kFrameCount = 53;

const std::array<std::string, kFrameCount> frame_keys = {
    "10_of_clubs.png",     "10_of_diamonds.png",     "10_of_hearts.png",     "10_of_spades.png",
    "2_of_clubs.png",      "2_of_diamonds.png",      "2_of_hearts.png",      "2_of_spades.png",
    "3_of_clubs.png",      "3_of_diamonds.png",      "3_of_hearts.png",      "3_of_spades.png",
    "4_of_clubs.png",      "4_of_diamonds.png",      "4_of_hearts.png",      "4_of_spades.png",
    "5_of_clubs.png",      "5_of_diamonds.png",      "5_of_hearts.png",      "5_of_spades.png",
    "6_of_clubs.png",      "6_of_diamonds.png",      "6_of_hearts.png",      "6_of_spades.png",
    "7_of_clubs.png",      "7_of_diamonds.png",      "7_of_hearts.png",      "7_of_spades.png",
    "8_of_clubs.png",      "8_of_diamonds.png",      "8_of_hearts.png",      "8_of_spades.png",
    "9_of_clubs.png",      "9_of_diamonds.png",      "9_of_hearts.png",      "9_of_spades.png",
    "ace_of_clubs.png",    "ace_of_diamonds.png",    "ace_of_hearts.png",    "ace_of_spades2.png",
    "jack_of_clubs2.png",  "jack_of_diamonds2.png",  "jack_of_hearts2.png",  "jack_of_spades2.png",
    "king_of_clubs2.png",  "king_of_diamonds2.png",  "king_of_hearts2.png",  "king_of_spades2.png",
    "queen_of_clubs2.png", "queen_of_diamonds2.png", "queen_of_hearts2.png", "queen_of_spades2.png",
    "cardback.png"};
} // namespace

TEST(JSONTests, CreateFrames_Success)
{
    auto frames_result = create_frames();
    ASSERT_TRUE(frames_result.has_value())
        << "Expected frames to be created successfully, but got error: " << frames_result.error();

    auto frames = frames_result.value();
    ASSERT_FALSE(frames.empty()) << "Created frames are empty.";

    const auto& item = frames["10_of_clubs.png"].first;
    EXPECT_EQ(item.x, 2);
    EXPECT_EQ(item.y, 2);
    EXPECT_EQ(item.w, 256);
    EXPECT_EQ(item.h, 372);
}

TEST(JSONTests, CreateFrames_CheckData)
{
    auto frames_result = create_frames();
    ASSERT_TRUE(frames_result.has_value())
        << "Expected frames to be created successfully, but got error: " << frames_result.error();

    auto frames = frames_result.value();
    ASSERT_FALSE(frames.empty()) << "Created frames are empty.";

    // For each value in frames, check that the atlas_pos_t value matches
    // the position in the frame_keys array
    for (const auto& item : frames)
    {
        const auto& key        = item.first;
        const auto& value_pair = item.second;
        const auto& pos        = value_pair.second;

        const auto& expected_key = frame_keys[pos];
        EXPECT_EQ(key, expected_key) << "Frame key mismatch for position " << pos;
    }
}

TEST(JSONTests, LoadJSONData_Success)
{
    auto json_result = load_json_data();
    ASSERT_TRUE(json_result.has_value())
        << "Expected JSON data to load successfully, but got error: " << json_result.error();

    auto json_data = json_result.value();
    ASSERT_FALSE(json_data.empty()) << "Loaded JSON data is empty.";
}
