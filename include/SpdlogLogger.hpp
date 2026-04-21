#ifndef SPDLOGLOGGER_HPP
#define SPDLOGLOGGER_HPP

#include "ILogger.hpp"

class SpdlogLogger : public ILogger
{
public:
    void logTrade(uint64_t bidID,
                  uint64_t askID,
                  double priceExectued,
                  int quantity) override;
};

#endif