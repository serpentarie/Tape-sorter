#pragma once
#include "tape.h"
#include "config/config.h"
#include <string>
#include <fstream>

class FileTape : public Tape
{
public:
    FileTape(const std::string &filePath, const Config &config);
    FileTape(const std::string &filePath, const Config &config, std::ios_base::openmode mode);
    ~FileTape();

    int32_t read() override;
    void write(int32_t value) override;
    void moveForward() override;
    void moveBackward() override;
    void rewindToStart() override;
    void rewindToEnd() override;
    size_t getPosition() const override;
    size_t getLength() const override;

private:
    std::string filePath_;
    std::fstream file_;
    size_t position_;
    size_t length_;
    const Config &config_;

    void checkFileOpen() const;
    void checkBounds(size_t pos) const;
    void applyDelay(int delayMs) const;
};