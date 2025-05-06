#include "file_tape.h"
#include <stdexcept>
#include <thread>
#include <chrono>
#include <iostream>

FileTape::FileTape(const std::string &filePath, const Config &config)
    : FileTape(filePath, config, std::ios::binary | std::ios::in | std::ios::out) {}

FileTape::FileTape(const std::string &filePath, const Config &config, std::ios_base::openmode mode)
    : filePath_(filePath), position_(0), config_(config)
{
    file_.open(filePath_, mode);
    checkFileOpen();

    file_.seekg(0, std::ios::end);
    auto fileSize = file_.tellg();
    length_ = fileSize / sizeof(int32_t);
    file_.seekg(0, std::ios::beg);
}

FileTape::~FileTape()
{
    if (file_.is_open())
    {
        file_.close();
    }
}

void FileTape::checkFileOpen() const
{
    if (!file_.is_open())
    {
        throw std::runtime_error("failed to open file");
    }
}

void FileTape::checkBounds(size_t pos) const
{
    if (pos >= length_)
    {
        throw std::runtime_error("position out of bounds");
    }
}

void FileTape::applyDelay(int delayMs) const
{
    if (delayMs > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

int32_t FileTape::read()
{
    checkFileOpen();
    if (length_ == 0)
    {
        throw std::runtime_error("tape is empty");
    }
    checkBounds(position_);
    file_.seekg(position_ * sizeof(int32_t));
    int32_t value;
    file_.read(reinterpret_cast<char *>(&value), sizeof(int32_t));
    if (!file_)
    {
        throw std::runtime_error("failed to read");
    }
    applyDelay(config_.getReadDelay());
    return value;
}

void FileTape::write(int32_t value)
{
    checkFileOpen();
    checkBounds(position_);
    file_.seekp(position_ * sizeof(int32_t));
    file_.write(reinterpret_cast<const char *>(&value), sizeof(int32_t));
    if (!file_)
    {
        throw std::runtime_error("failed to write");
    }
    applyDelay(config_.getWriteDelay());
}

void FileTape::moveForward()
{
    checkFileOpen();
    if (position_ + 1 > length_)
    {
        position_ = length_;
        return;
    }
    ++position_;
    applyDelay(config_.getMoveDelay());
}

void FileTape::moveBackward()
{
    checkFileOpen();
    if (position_ == 0)
    {
        throw std::runtime_error("start of the tape");
    }
    --position_;
    applyDelay(config_.getMoveDelay());
}

void FileTape::rewindToStart()
{
    checkFileOpen();
    position_ = 0;
    file_.seekg(0, std::ios::beg);
    applyDelay(config_.getRewindDelay());
}

void FileTape::rewindToEnd()
{
    checkFileOpen();
    if (length_ > 0)
    {
        position_ = length_ - 1;
        file_.seekg(position_ * sizeof(int32_t));
    }
    else
    {
        position_ = 0;
        file_.seekg(0, std::ios::beg);
    }
    applyDelay(config_.getRewindDelay());
}

size_t FileTape::getPosition() const { return position_; }
size_t FileTape::getLength() const { return length_; }