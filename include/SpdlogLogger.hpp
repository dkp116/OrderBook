#ifndef SPDLOGLOGGER_HPP
#define SPDLOGLOGGER_HPP

#include "ILogger.hpp"

class SpdlogLogger : public ILogger
{
public:
    void logTrade(double bidPrice,
                  double askPrice,
                  int quantity) override;
};

#endif