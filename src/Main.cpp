#include "sort/sorter.h"
#include "config/config.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return 1;
    }

    std::string inputTapePath = argv[1];
    std::string outputTapePath = argv[2];

    fs::create_directories("tmp");

    Config config("config.json");
    size_t memoryLimitBytes = 1024 * 1024;
    Sorter sorter(config, memoryLimitBytes);
    sorter.sort(inputTapePath, outputTapePath);
}