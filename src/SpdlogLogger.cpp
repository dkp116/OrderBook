#include "ILogger.hpp"
#include "spdlog/spdlog.h"

class SpdlogLogger : public ILogger
{
public:
    void logTrade(double bidPrice,
                  double askPrice,
                  int quantity) override
    {
        spdlog::info(
            "TRADE | BUY @ {} <-> SELL @ {} | QTY {}",
            bidPrice, askPrice, quantity
        );
    }
};