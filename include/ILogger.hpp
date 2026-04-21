#ifndef ILOGGER_HPP
#define ILOGGER_HPP
#include <cstdint>

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void logTrade(uint64_t bidID,
                          uint64_t askID,
                          double priceExectued,
                          int quantity) = 0;
};

#endif