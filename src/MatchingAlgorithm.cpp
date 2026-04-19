#include "MatchingAlgorithm.hpp"

bool MatchingAlgorithm::anyOrdersToMatch()
{
    if (!Bids.empty() && !Asks.empty())
    {
        return true;
    }
    return false;
}

bool MatchingAlgorithm::validateOrdersToMatch()
{
    Order topBuyOrder = Bids.front();
    Order topAskOrder = Asks.front();
    if (topBuyOrder.getPrice() >= topAskOrder.getPrice())
    {
        return true;
    }
    return false;
}

void MatchingAlgorithm::FillOrder(){
    Order& topBuyOrder = Bids.front();           
    Order& topAskOrder = Asks.front();
    double remainingQuantity = topBuyOrder.getQuantity() - topAskOrder.getQuantity();
    topBuyOrder.newQuantity(remainingQuantity);
    topAskOrder.newQuantity(-remainingQuantity);
}

void MatchingAlgorithm::CleanOrders()
{
    while (!Bids.empty() && Bids.front().getQuantity() <= 0)
    {
        Bids.pop_front();
    }

    while (!Asks.empty() && Asks.front().getQuantity() <= 0)
    {
        Asks.pop_front();
    }
}
