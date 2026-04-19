#ifndef MATCHINGALGORITHM_HPP
#define MATCHINGALGORITHM_HPP

#include "Order.hpp"
#include <deque>

class MatchingAlgorithm
{
private:
    std::deque<Order>& Bids;
    std::deque<Order>& Asks;

public:
    MatchingAlgorithm(std::deque<Order>& Bids_, std::deque<Order>& Asks_) : Bids(Bids_), Asks(Asks_) {};
    bool anyOrdersToMatch();
    bool validateOrdersToMatch();
    void FillOrder();
    void CleanOrders();
    void matchTopOfBook();
};

#endif