#ifndef MATCHINGALGORITHM_HPP
#define MATCHINGALGORITHM_HPP

#include "Order.hpp"
#include <deque>

class MatchingAlgorithm
{
public:
    MatchingAlgorithm() {};
    bool anyOrdersToMatch(std::deque<Order> &Bids, std::deque<Order> &Asks);
    bool validateOrdersToMatch(std::deque<Order> &Bids, std::deque<Order> &Asks);
};

#endif