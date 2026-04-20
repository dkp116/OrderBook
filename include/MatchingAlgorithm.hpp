#ifndef MATCHINGALGORITHM_HPP
#define MATCHINGALGORITHM_HPP

#include "Order.hpp"
#include "SpdlogLogger.hpp"
#include <deque>
#include<memory>

class MatchingAlgorithm
{
private:
    std::deque<Order>& Bids;
    std::deque<Order>& Asks;
    std::shared_ptr<ILogger> logger;

public:
    MatchingAlgorithm(std::deque<Order>& Bids_, std::deque<Order>& Asks_ , std::shared_ptr<ILogger> logger_) : Bids(Bids_), Asks(Asks_), logger(logger_) {};
    bool anyOrdersToMatch();
    bool validateOrdersToMatch();
    void FillOrder();
    void MakeQuantityNonNegative(double &remainingQuantityBids, double &remainingQuanityAsks);
    void CleanOrders();
    void matchTopOfBook();
    void LogTrade(double bidPrice, double askPrice,int quantity);
};

#endif