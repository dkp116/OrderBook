#include "SpdlogLogger.hpp"
#include "spdlog/spdlog.h"

void SpdlogLogger::logTrade(uint64_t bidID,
                            uint64_t askID,
                            double priceExectued,
                            int quantity)
{
    spdlog::info(
        "TRADE | BUY {} matched with SELL {} @ {}| QTY {}",
        bidID, askID, priceExectued, quantity
    );
}