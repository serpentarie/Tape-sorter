#include "config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

namespace
{
    constexpr int DEFAULT_READ_DELAY_MS = 1;
    constexpr int DEFAULT_WRITE_DELAY_MS = 1;
    constexpr int DEFAULT_MOVE_DELAY_MS = 1;
    constexpr int DEFAULT_REWIND_DELAY_MS = 1;
}

Config::Config(const std::string &configPath)
    : readDelayMs_(DEFAULT_READ_DELAY_MS),
      writeDelayMs_(DEFAULT_WRITE_DELAY_MS),
      moveDelayMs_(DEFAULT_MOVE_DELAY_MS),
      rewindDelayMs_(DEFAULT_REWIND_DELAY_MS)
{
    std::ifstream file(configPath);

    nlohmann::json configJson;
    file >> configJson;
    if (configJson.contains("read_delay_ms") && configJson["read_delay_ms"].is_number_integer())
    {
        readDelayMs_ = configJson["read_delay_ms"];
    }
    if (configJson.contains("write_delay_ms") && configJson["write_delay_ms"].is_number_integer())
    {
        writeDelayMs_ = configJson["write_delay_ms"];
    }
    if (configJson.contains("move_delay_ms") && configJson["move_delay_ms"].is_number_integer())
    {
        moveDelayMs_ = configJson["move_delay_ms"];
    }
    if (configJson.contains("rewind_delay_ms") && configJson["rewind_delay_ms"].is_number_integer())
    {
        rewindDelayMs_ = configJson["rewind_delay_ms"];
    }
}

int Config::getReadDelay() const { return readDelayMs_; }
int Config::getWriteDelay() const { return writeDelayMs_; }
int Config::getMoveDelay() const { return moveDelayMs_; }
int Config::getRewindDelay() const { return rewindDelayMs_; }