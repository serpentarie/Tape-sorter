#pragma once
#include <string>
class Config
{
public:
    Config(const std::string &configPath);

    int getReadDelay() const;
    int getWriteDelay() const;
    int getMoveDelay() const;
    int getRewindDelay() const;

private:
    int readDelayMs_;
    int writeDelayMs_;
    int moveDelayMs_;
    int rewindDelayMs_;
};