#include "SpdlogLogger.hpp"
#include "spdlog/spdlog.h"

void SpdlogLogger::logTrade(double bidPrice,
                            double askPrice,
                            int quantity)
{
    spdlog::info(
        "TRADE | BUY @ {} <-> SELL @ {} | QTY {}",
        bidPrice, askPrice, quantity
    );
}