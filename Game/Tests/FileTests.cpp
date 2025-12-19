#include "File.hpp"

#include <gtest/gtest.h>

TEST(FileTest, ReadFileContent_Success)
{
    auto current_working_path = std::filesystem::current_path();
    auto test_file_path       = current_working_path / "Game/Tests/TestData/TestFile.txt";

    auto content_result = read_file_content(test_file_path);
    ASSERT_TRUE(content_result.has_value())
        << "Expected file content to be read successfully, but got error: "
        << content_result.error();

    auto content = content_result.value();
    EXPECT_EQ(content, "This is a test file.\nIt has multiple lines.\n");
}
