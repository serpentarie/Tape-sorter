#include "sorter.h"
#include "tape/file_tape.h"
#include <algorithm>
#include <filesystem>
#include <queue>
#include <limits>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>

Sorter::Sorter(const Config &config, size_t memoryLimitBytes)
    : config_(config), memoryLimitBytes_(memoryLimitBytes)
{
}

void Sorter::sort(const std::string &inputTapePath, const std::string &outputTapePath)
{
    std::vector<std::string> tempTapePaths;
    createInitialRuns(inputTapePath, tempTapePaths);
    mergeRuns(tempTapePaths, outputTapePath);

    for (const auto &path : tempTapePaths)
    {
        std::filesystem::remove(path);
    }
}

void Sorter::createInitialRuns(const std::string &inputTapePath, std::vector<std::string> &tempTapePaths)
{
    auto inputTape = std::make_unique<FileTape>(inputTapePath, config_);
    size_t elementSize = sizeof(int32_t);
    size_t maxElementsInMemory = memoryLimitBytes_ / elementSize;

    std::vector<int32_t> buffer;
    buffer.reserve(maxElementsInMemory);
    size_t tempTapeIndex = 0;

    std::filesystem::create_directory("tmp");

    while (inputTape->getPosition() < inputTape->getLength())
    {
        buffer.clear();
        size_t elementsRead = 0;
        while (buffer.size() < maxElementsInMemory && inputTape->getPosition() < inputTape->getLength())
        {
            buffer.push_back(inputTape->read());
            elementsRead++;
            size_t newPosition = inputTape->getPosition() + 1;
            if (newPosition < inputTape->getLength())
            {
                inputTape->moveForward();
            }
            else
            {
                inputTape->rewindToEnd();
                inputTape->moveForward();
            }
        }

        std::sort(buffer.begin(), buffer.end());

        std::string tempTapePath = "tmp/run_" + std::to_string(tempTapeIndex++) + ".bin";
        {
            std::ofstream tempFile(tempTapePath, std::ios::binary);
            if (!tempFile.is_open())
            {
                throw std::runtime_error(std::strerror(errno));
            }
            tempFile.write(reinterpret_cast<const char *>(buffer.data()), buffer.size() * elementSize);
            tempFile.flush();
            tempFile.close();
        }
        std::filesystem::permissions(tempTapePath, std::filesystem::perms::owner_read | std::filesystem::perms::owner_write, std::filesystem::perm_options::add);
        auto tempTape = std::make_unique<FileTape>(tempTapePath, config_);
        tempTape->rewindToStart();
        tempTapePaths.push_back(tempTapePath);
    }
}

void Sorter::mergeRuns(const std::vector<std::string> &tempTapePaths, const std::string &outputTapePath)
{
    size_t inputLength = 0;
    {
        auto inputTape = std::make_unique<FileTape>(tempTapePaths[0], config_, std::ios::binary | std::ios::in);
        inputLength = inputTape->getLength();
    }
    {
        std::ofstream outputFile(outputTapePath, std::ios::binary);
        if (!outputFile.is_open())
        {
            throw std::runtime_error(std::strerror(errno));
        }
        outputFile.seekp(inputLength * sizeof(int32_t) - 1);
        outputFile.write("\0", 1);
        outputFile.flush();
        outputFile.close();
    }
    auto outputTape = std::make_unique<FileTape>(outputTapePath, config_);

    struct TapeElement
    {
        int32_t value;
        size_t tapeIndex;
        bool operator>(const TapeElement &other) const { return value > other.value; }
    };

    std::priority_queue<TapeElement, std::vector<TapeElement>, std::greater<TapeElement>> pq;
    std::vector<std::unique_ptr<Tape>> tapes;

    for (size_t i = 0; i < tempTapePaths.size(); ++i)
    {
        if (!std::filesystem::exists(tempTapePaths[i]))
        {
            throw std::runtime_error(tempTapePaths[i]);
        }
        try
        {
            auto tape = std::make_unique<FileTape>(tempTapePaths[i], config_, std::ios::binary | std::ios::in);
            if (tape->getLength() > 0)
            {
                pq.push({tape->read(), i});
                if (tape->getPosition() < tape->getLength() - 1)
                {
                    tape->moveForward();
                }
            }
            tapes.push_back(std::move(tape));
        }
        catch (const std::exception &e)
        {
            throw std::runtime_error(std::strerror(errno));
        }
    }

    size_t elementsWritten = 0;
    while (!pq.empty())
    {
        if (elementsWritten >= outputTape->getLength())
        {
            break;
        }

        auto elem = pq.top();
        pq.pop();

        outputTape->write(elem.value);
        elementsWritten++;
        if (elementsWritten < outputTape->getLength())
        {
            outputTape->moveForward();
        }

        auto &tape = tapes[elem.tapeIndex];
        if (tape->getPosition() < tape->getLength())
        {
            pq.push({tape->read(), elem.tapeIndex});
            tape->moveForward();
        }
    }
}

std::unique_ptr<Tape> Sorter::createTempTape(const std::string &tempTapePath)
{
    return std::make_unique<FileTape>(tempTapePath, config_);
}