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
    double LogOrder();      //maybe make a logger class that we will use to log the difference transactions
};

#endif