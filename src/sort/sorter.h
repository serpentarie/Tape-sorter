#pragma once
#include "tape/tape.h"
#include "config/config.h"
#include <string>
#include <memory>
#include <vector>

class Sorter
{
public:
    Sorter(const Config &config, size_t memoryLimitBytes);
    void sort(const std::string &inputTapePath, const std::string &outputTapePath);

private:
    const Config &config_;
    size_t memoryLimitBytes_;

    void createInitialRuns(const std::string &inputTapePath, std::vector<std::string> &tempTapePaths);
    void mergeRuns(const std::vector<std::string> &tempTapePaths, const std::string &outputTapePath);
    std::unique_ptr<Tape> createTempTape(const std::string &tempTapePath);
};