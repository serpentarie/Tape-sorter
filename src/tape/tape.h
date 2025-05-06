#pragma once
#include <cstdint>
#include <cstddef>

class Tape
{
public:
    virtual ~Tape() = default;
    virtual int32_t read() = 0;
    virtual void write(int32_t value) = 0;
    virtual void moveForward() = 0;
    virtual void moveBackward() = 0;
    virtual void rewindToStart() = 0;
    virtual void rewindToEnd() = 0;
    virtual size_t getPosition() const = 0;
    virtual size_t getLength() const = 0;
};