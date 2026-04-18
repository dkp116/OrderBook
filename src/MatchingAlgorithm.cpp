#include "MatchingAlgorithm.hpp"

bool MatchingAlgorithm::anyOrdersToMatch(std::deque<Order> &Bids, std::deque<Order> &Asks)
{
    if (!Bids.empty() && !Asks.empty())
    {
        return true;
    }
    return false;
}

bool MatchingAlgorithm::validateOrdersToMatch(std::deque<Order> &Bids, std::deque<Order> &Asks)
{
    Order topBuyOrder = Bids.front();
    Order topAskOrder = Asks.front();
    if (topBuyOrder.getPrice() >= topAskOrder.getPrice())
    {
        return true;
    }
    return false;
}
