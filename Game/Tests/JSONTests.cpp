#include "JSON.hpp"

#include <gtest/gtest.h>

TEST(JSONTests, CreateFrames_Success)
{
    auto frames_result = create_frames();
    ASSERT_TRUE(frames_result.has_value())
        << "Expected frames to be created successfully, but got error: " << frames_result.error();

    auto frames = frames_result.value();
    ASSERT_FALSE(frames.empty()) << "Created frames are empty.";

    const auto& item = frames["10_of_clubs.png"];
    EXPECT_EQ(item.x, 2);
    EXPECT_EQ(item.y, 2);
    EXPECT_EQ(item.w, 256);
    EXPECT_EQ(item.h, 372);
}

TEST(JSONTests, LoadJSONData_Success)
{
    auto json_result = load_json_data();
    ASSERT_TRUE(json_result.has_value())
        << "Expected JSON data to load successfully, but got error: " << json_result.error();

    auto json_data = json_result.value();
    ASSERT_FALSE(json_data.empty()) << "Loaded JSON data is empty.";
}
