#include <gtest/gtest.h>
#include "sort/sorter.h"
#include <fstream>
#include <vector>
#include <filesystem>

TEST(SorterTest, SortSmallInput)
{
    Config config("config.json");
    Sorter sorter(config, 1024 * 1024);
    std::vector<int32_t> input = {5, 2, 8, 1};
    std::ofstream inputFile("test_input1.bin", std::ios::binary);
    inputFile.write(reinterpret_cast<const char *>(input.data()), input.size() * sizeof(int32_t));
    inputFile.close();

    sorter.sort("test_input1.bin", "test_output1.bin");

    std::ifstream outputFile("test_output.bin", std::ios::binary);
    std::vector<int32_t> output(4);
    outputFile.read(reinterpret_cast<char *>(output.data()), 4 * sizeof(int32_t));
    outputFile.close();

    std::vector<int32_t> expected = {1, 2, 5, 8};
    ASSERT_EQ(output, expected);
}

TEST(SorterTest, SortThirtyElements)
{
    std::filesystem::remove_all("tmp");
    Config config("config.json");
    Sorter sorter(config, 1024 * 1024);

    std::vector<int32_t> input = {
        45, 12, 78, 23, 56, 89, 34, 67, 90, 11,
        25, 48, 73, 19, 62, 37, 81, 50, 94, 28,
        70, 15, 83, 41, 96, 32, 64, 87, 20, 53};
    std::ofstream inputFile("test_input2.bin", std::ios::binary);
    inputFile.write(reinterpret_cast<const char *>(input.data()), input.size() * sizeof(int32_t));
    inputFile.close();

    sorter.sort("test_input2.bin", "test_output2.bin");

    std::ifstream outputFile("test_output2.bin", std::ios::binary);
    std::vector<int32_t> output(30);
    outputFile.read(reinterpret_cast<char *>(output.data()), 30 * sizeof(int32_t));
    outputFile.close();

    std::vector<int32_t> expected = {
        11, 12, 15, 19, 20, 23, 25, 28, 32, 34,
        37, 41, 45, 48, 50, 53, 56, 62, 64, 67,
        70, 73, 78, 81, 83, 87, 89, 90, 94, 96};
    ASSERT_EQ(output, expected);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}